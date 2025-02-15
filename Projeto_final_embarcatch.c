
/* 
Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
*
* SPDX-License-Identifier: MIT
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
    add_repeating_timer_ms(2000, repeating_timer_callback, NULL, &timer);

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