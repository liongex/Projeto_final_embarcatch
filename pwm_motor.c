/*
    Atividade 7 - Embarcatech
    Aluno: Isac de Lima Feliciano;
    Matrícula: 202421511720213.
    
    BIBLIOTECA RESPONSÁVEL PELO PWM DO MOTOR.

*/


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pwm_motor.h"



void setup_pwm(int PIN, float DIVIDER_PWM, uint16_t PERIOD, uint16_t pwm_level)
{
    uint slice;
    gpio_set_function(PIN, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    slice = pwm_gpio_to_slice_num(PIN);    // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(slice, DIVIDER_PWM);    // Define o divisor de clock do PWM
    pwm_set_wrap(slice, PERIOD);           // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(PIN, pwm_level);    // Define o nível inicial do PWM para o pino do LED
    pwm_set_enabled(slice, true);          // Habilita o PWM no slice correspondente
};

void update_pwm(int PIN, uint16_t* pwm_new_level, float indiceDeCalor)
{
    // Define a faixa de valores do Índice de Calor
    float minIndice = 28.0;
    float maxIndice = 45.0;

    // Calcula o duty cycle com base na faixa de valores
    float dutyCycle;
    if (indiceDeCalor <= minIndice) {
        dutyCycle = 0.5; // 50% de duty cycle
    } else if (indiceDeCalor >= maxIndice) {
        dutyCycle = 1.0; // 100% de duty cycle
    } else {
        // Mapeia o Índice de Calor para a faixa de 50% a 100%
        dutyCycle = 0.5 + ((indiceDeCalor - minIndice) / (maxIndice - minIndice) * 0.5);
    }

    // Calcula o nível de PWM correspondente
    *pwm_new_level = (uint16_t)(dutyCycle * 999);

    // Define o nível do PWM
    pwm_set_gpio_level(PIN, *pwm_new_level);

};

void update_pwm2(int PIN, uint16_t* pwm_new_level)
{
    // Define o nível do PWM
    pwm_set_gpio_level(PIN, *pwm_new_level);

}
