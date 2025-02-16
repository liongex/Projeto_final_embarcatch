
/* 
UNIDADE 07 EMBARCATECH
ALUNO: ISAC DE LIMA FELICIANO.
MATRÍCULA: 202421511720213.

    O PRESENTE TRABALHO TEM COMO OBJETIVO IMPLEMENTAR UM SISTEMA IOT PARA O CONTROLE DA VELOCIDADE
DE UM VENTILADOR A PARTIR DO ÍNDICE DE CALOR DO AMBIENTE. O SISTEMA CONTA COM COMUNICAÇÃO WI-FI COM USO DO PROTOCOLO
MQTT QUE ENVIA OS VALORES DA TEMPERATURA, UMIDADE, CONFORTO TÉRMICO E VELOCIDADE DO VENTILADOR PARA UM BROKER
PÚBLICO, ONDE O USUÁRIO PODE VERIFICAR OS DADOS. NO DISPLAY DO DISPOSITIVO, O USUÁRIO TAMBÉM PODE VERIFICAR ESSES 
VALORES EM UM MENU E SELECIONAR ENTRE O CONTROLE DE VELOCIDADE AUTOMÁTICO OU MANUAL. O CONTROLE MANUAL 
TEM SUA VELOCIDADE AJUSATADA ATRAVÉS DO USO DOS BOTÕES. ALÉM DISSO, A MATRIZ DE LEDS EXIBE UMA ESCALA INTUITIVA
E CHAMATIVA QUE SE RELACIONA COM A INDUICAÇÃO DO CONFORTO TÉRMICO. DESSA FORMA, É FACILITADA A COMPREENSÃO DOS 
VALORES OBTIDOS PELO USUÁRIO.
*/

#include <dht.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include "pwm_motor.h"

#define PWM_PIN 19

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
char* determinarConfortoTermico(float indiceDeCalor);
static float celsius_to_fahrenheit(float temperature);
void medir_dht();
float calcularIndiceDeCalor();
bool repeating_timer_callback(struct repeating_timer *t);



int main() {

    setup();

    add_repeating_timer_ms(2000, repeating_timer_callback, NULL, &timer);//Timer de 2 segundos para a realização da medição;


    do{

        if(semaforo){

            update_pwm(PWM_PIN, &pwm_new_level, indice);
            semaforo = 0;
            printf("valor do pwm = %d\n", pwm_new_level);
        }

        sleep_ms(50);

    }while(true);

};

//função para configuração
void setup(){
    stdio_init_all();
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */); // Inicialização do sensor DHT11
    setup_pwm(PWM_PIN,DIVIDER_PWM, PERIOD, 500); // Inicialização do pwm
};

//Função para determinar om conforto térmico
char* determinarConfortoTermico(float indiceDeCalor) { 
    if (indiceDeCalor < 28.0) { 
        return "CONFORTAVEL"; 
    } else if (indiceDeCalor < 33.0) { 
        return "MOD. DESCONF."; 
    } else if (indiceDeCalor < 38.0) { 
        return "DESCONFORTAVEL"; 
    } else if (indiceDeCalor < 45.0) { 
        return "PERIGOSO"; 
    } else { 
        return "EXTR. PERIGOSO"; 
    }
}

//Função para conversão de graus celsius para fahrenheit
static float celsius_to_fahrenheit(float temperature) {
    return temperature * (9.0f / 5) + 32;
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

//Função para calcualr o índice de calor
 float calcularIndiceDeCalor() { 
    return -8.784695 + 1.61139411 * temperature_c + 2.338549 * humidity - 0.14611605 * temperature_c * humidity
    - 0.012308094 * (temperature_c * temperature_c) - 0.016424828 * (humidity * humidity)
     + 0.002211732 * (temperature_c * temperature_c) * humidity + 0.00072546 * temperature_c * (humidity * humidity) 
     - 0.000003582 * (temperature_c * temperature_c) * (humidity * humidity);
     };

//Função, de callback para o timer, que realiza medição de temperatura e umidade
bool repeating_timer_callback(struct repeating_timer *t) {

    medir_dht();
    indice = calcularIndiceDeCalor();
    conforto = determinarConfortoTermico(indice);
    printf("%s\n", conforto);
    return true;

}

