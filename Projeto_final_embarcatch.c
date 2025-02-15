
/* 
UNIDADE 07 EMBARCATECH
ALUNO: ISAC DE LIMA FELICIANO.
MATRÍCULA: 202421511720213.

    O PRESENTE TRABALHO TEM COMO OBJETIVO IMPLEMENTAR UM SISTEMA IOT PARA O CONTROLE DA VELOCIDADE
DE UM VENTILADOR A PARTIR DO ÍNDICE DE CALOR DO AMBIENTE. O SISTEMA CONTA COM COMUNICAÇÃO WI-FI VIA MQTT
QUE ENVIA OS VALOR DA TEMPERATURA, UMIDADE, CONFORTE TÉRMICO E VELOCIDADE DO VENTILADOR PARA UM BROKER PÚBLICO
ONDE O USUÁRIO PODE VERIFICAR OS DADOS. NO VISOR DO DISPOSITIVO, O USUÁRIO PODE SELECIONAR ENTRE 
MODO AUTOMÁRICO E MANUAL PARA O CONTROLE DA VELOCIDADE DO MOTOR DO VENTILADOR. ALÉM DISSO, A MATRIZ DE LEDS
EXIBE UMA ESCALA INTUITIVA E CHAMATIVA QUE INDICA O CONFORTO TÉRMICO, DESSA FORMA FACILITANDO A COMPREENSÃO DOS VALORES OBTIDOS.
*/

#include <dht.h>
#include <pico/stdlib.h>
#include <stdio.h>

// change this to match your setup
static const dht_model_t DHT_MODEL = DHT11;
static const uint DATA_PIN = 18;

dht_t dht;

float humidity;
float temperature_c;

struct repeating_timer timer;

void setup();
static float celsius_to_fahrenheit(float temperature);
void medir_dht();
float calcularIndiceDeCalor();
bool repeating_timer_callback(struct repeating_timer *t);

int main() {
    
    setup();

    add_repeating_timer_ms(2000, repeating_timer_callback, NULL, &timer);//Timer de 2 segundos para a realização da medição;

    do{
        tight_loop_contents();


    }while(true);

};

void setup(){
    stdio_init_all();
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */); // Inicialização do sensor DHT11
};

static float celsius_to_fahrenheit(float temperature) {
    return temperature * (9.0f / 5) + 32;
 };

 void medir_dht(){

    dht_start_measurement(&dht);

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

 float calcularIndiceDeCalor() { 
    return -8.784695 + 1.61139411 * temperature_c + 2.338549 * humidity - 0.14611605 * temperature_c * humidity
    - 0.012308094 * (temperature_c * temperature_c) - 0.016424828 * (humidity * humidity)
     + 0.002211732 * (temperature_c * temperature_c) * humidity + 0.00072546 * temperature_c * (humidity * humidity) 
     - 0.000003582 * (temperature_c * temperature_c) * (humidity * humidity);
     };

bool repeating_timer_callback(struct repeating_timer *t) {
        medir_dht();
        return true;
    }