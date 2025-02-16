
/* 
UNIDADE 07 EMBARCATECH
ALUNO: ISAC DE LIMA FELICIANO.
MATRÍCULA: 202421511720213.

    O PRESENTE TRABALHO TEM COMO OBJETIVO IMPLEMENTAR UM SISTEMA IOT PARA O CONTROLE DA VELOCIDADE
DE UM VENTILADOR A PARTIR DO ÍNDICE DE CALOR DO AMBIENTE. O SISTEMA CONTA COM COMUNICAÇÃO WI-FI COM USO DO PROTOCOLO
HTTP QUE ENVIA OS VALORES DA TEMPERATURA, UMIDADE E ÍNDICE TÉRMICO PARA O THINGSPEAK, ONDE O USUÁRIO PODE VERIFICAR
OS DADOS. NO DISPLAY DO DISPOSITIVO, O USUÁRIO TAMBÉM PODE VERIFICAR ESSES VALORES EM UM MENU E SELECIONAR 
ENTRE O CONTROLE DE VELOCIDADE AUTOMÁTICO OU MANUAL. O CONTROLE MANUAL TEM SUA VELOCIDADE AJUSATADA ATRAVÉS DO 
USO DOS BOTÕES. ALÉM DISSO, O LED EXIBE UMA ESCALA INTUITIVA E CHAMATIVA DE COR QUE SE RELACIONA COM A 
INDUICAÇÃO DO CONFORTO TÉRMICO. DESSA FORMA, É FACILITADA A COMPREENSÃO DOS VALORES OBTIDOS PELO USUÁRIO.
*/

#include <dht.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include "pwm_motor.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "display.h"
#include "temperatura.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"

#define PWM_PIN 19

#define BOTTON_A 5
#define BOTTON_B 6

#define I2C_PORT i2c1 // módulo i2c usado
#define I2C_SDA 14 // pino para comunicação i2c
#define I2C_SCL 15// pino para comunicação i2c
#define LED_B 12 // pino led azul
#define LED_R 13 // pino led vermelho
#define LED_G 11 //pino led verde
#define SW 22 //Pino do Botão do Joystick
#define VRY 26 //Porta ADC de variação do Y do Joystick
#define VRX 27 //Porta ADC de variação do X do joystick

// Define o tempo de debounce em milissegundos
#define DEBOUNCE_TIME_MS 50

ssd1306_t disp;//variável display do Oled
static volatile bool ativo = 0;
volatile uint32_t last_interrupt_time = 0;

static uint menu = 1;
static uint case1 = 0;
static uint case2 = 0;

const int ADC_CHANNEL_0 = 0; // Canal ADC para o eixo X do joystick
const int ADC_CHANNEL_1 = 1; // Canal ADC para o eixo Y do joystick

static const dht_model_t DHT_MODEL = DHT11; //variáveis necessárias para a biblioteca do dht
static const uint DATA_PIN = 18;//variáveis necessárias para a biblioteca do dht

dht_t dht; //variáveis necessárias para a biblioteca do dht

const float DIVIDER_PWM = 1250.0;  // Divisor fracional do clock para o PWM
const uint16_t PERIOD = 999;   //Período para o pwm
static uint16_t pwm_new_level = 0; 

float humidity;
float temperature_c;
char* conforto = "Não calculado ainda";
float indice;

bool semaforo = false;

struct repeating_timer timer; // varíavel para armazenamento do tempo do temporizador

//Funções declaradas no código
void setup();
void medir_dht();
bool repeating_timer_callback(struct repeating_timer *t);
void setup_leds();// função para configuração padrão dos leds
static void gpio_irq_handler(uint gpio, uint32_t events);// função de callback da interrupção



int main() {

    setup();
    setup_leds();
    uint countdown = 0; //verificar seleções para baixo do joystick
    uint countup = 1; //verificar seleções para cima do joystick
    uint pos_y=12; //inicialização de variável para ler posição do Y do Joystick
    uint posy_ant=12;//posição anterior
    uint16_t pwm_new_manual = 500;

    print_menu(pos_y, &disp);//impressão inicial do menu

    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Função de interrupção para controle do menu através do botão

    add_repeating_timer_ms(2000, repeating_timer_callback, NULL, &timer);//Timer de 2 segundos para a realização da medição;


    do{

        if(semaforo == true && menu == 2){

            update_pwm(PWM_PIN, &pwm_new_level, indice);
            semaforo = 0;
            printf("valor do pwm = %d\n", pwm_new_level);
            case2 = 0;
        } if(ativo == 0) {     
            adc_select_input(ADC_CHANNEL_1);
            uint adc_y_raw = adc_read();//leitura do joystick
            const uint bar_width = 40;
            const uint adc_max = (1 << 12) - 1;
            uint bar_y_pos = adc_y_raw * bar_width / adc_max; //bar_y_pos determinará se o Joystick foi pressionado para cima ou para baixo
            //o valor de 19 é o estado de repouso do Joystick da minha placa
            if(bar_y_pos < 19 && countdown <1 ){
                pos_y+=12;
                countdown+=1;
                countup-=1;
                menu++;//incrementa menu
            }else if(bar_y_pos > 19 && countup <1){
                pos_y-=12;
                countup+=1;
                countdown-=1;
                menu--;//decrementa menu
            }
            //texto do Menu
            if(pos_y!=posy_ant){//verifica se houve mudança de posição no menu.
                print_menu(pos_y, &disp);
            }
        }
         //verifica se botão foi pressionado. 
         //Se sim, entra no switch case para verificar posição do seletor e chama acionamento dos leds.
        if(ativo){
            switch (menu){
            case 1:
                if(case1 == 0){
                    print_menu2(pos_y, &disp, pwm_new_manual);
                    update_pwm2(PWM_PIN,&pwm_new_manual );
                    case1 = 1;
                }else{
                    if(!gpio_get(BOTTON_B)){ //Bottão B para aumentar a velocidade
                        if(pwm_new_manual < 999){
                            pwm_new_manual += 20;

                        }else{
                            pwm_new_manual = 999;
                        }
                        update_pwm2(PWM_PIN,&pwm_new_manual );
                        case1 = 0;
                    }  else  if(!gpio_get(BOTTON_A)){ //Bottão A para diminuir a velocidade
                        if(pwm_new_manual > 150){
                            pwm_new_manual -= 20;

                        }else{
                            pwm_new_manual = 150;
                        }
                        update_pwm2(PWM_PIN,&pwm_new_manual );
                        case1 = 0;
                    } ; 

                }
                
            break;
            case 2:
                if(case2 == 0){
                print_menu3(pos_y, &disp, pwm_new_level, temperature_c, humidity, conforto);
                case2 = 1;
                }
                Atualizar_cor(indice);
            break;
            default:
                printf("erro\n");
            break;
            }
       }
        sleep_ms(100);//delay de atualização
        posy_ant=pos_y;//atualização posição anterior.

    }while(true);

};

//função para configuração
void setup(){
    stdio_init_all();
    
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */); // Inicialização do sensor DHT11
    setup_pwm(PWM_PIN,DIVIDER_PWM, PERIOD, 500); // Inicialização do pwm

    //inicialização do Oled
    stdio_init_all();
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&disp);
    ssd1306_invert(&disp, true);

    //inicialização do botão do joystick
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);

    //Inicializar ADC do joystick
    adc_init();
    adc_gpio_init(VRY);
    adc_gpio_init(VRX);

    //inicialização dos botões A e B
    gpio_init(BOTTON_A);
    gpio_init(BOTTON_B);
    gpio_set_dir(BOTTON_A, GPIO_IN);
    gpio_set_dir(BOTTON_B, GPIO_IN);
    gpio_pull_up(BOTTON_A);
    gpio_pull_up(BOTTON_B);
};

 //Função para obter os valores de temperatura e umidade do sensor
 void medir_dht(){

    dht_start_measurement(&dht);
    semaforo = true;

    dht_result_t result = dht_finish_measurement_blocking(&dht, &humidity, &temperature_c);
    if (result == DHT_RESULT_OK) {
        printf("%.1f C (%.1f F), %.1f%% humidity\n", temperature_c, celsius_to_fahrenheit(temperature_c), humidity);
    } else if (result == DHT_RESULT_TIMEOUT) {
        puts("DHT sensor not responding. Please check your wiring.");
    } else {
        assert(result == DHT_RESULT_BAD_CHECKSUM);
        puts("Bad checksum");
    }
    
 };

//Função, de callback para o timer, que realiza medição de temperatura e umidade
bool repeating_timer_callback(struct repeating_timer *t) {

    medir_dht();
    indice = calcularIndiceDeCalor(temperature_c,humidity);
    conforto = determinarConfortoTermico(indice);
    printf("%s\n", conforto);
    return true;

}

//inicializa leds
void setup_leds(){

    gpio_init(LED_B);
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_B, false);
    gpio_put(LED_R, false);
    gpio_put(LED_G, false);


};

//função da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events){
        
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        if ((current_time - last_interrupt_time) > DEBOUNCE_TIME_MS) {
            // Código a ser executado quando o botão é pressionado
            ativo = !ativo;
            case1 = 0;
            case2 = 0;
            setup_leds();
            // Atualizar o tempo da última interrupção
            last_interrupt_time = current_time;
        }
};




