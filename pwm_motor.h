/*
    Atividade 7 - Embarcatech
    Aluno: Isac de Lima Feliciano;
    Matrícula: 202421511720213.
    
    BIBLIOTECA RESPONSÁVEL PELO PWM DO MOTOR.

*/

#ifndef _inc_pwm
#define _inc_pwm

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Função para configurar o PWM de um LED (genérica para verde)
void setup_pwm(int PIN, float DIVIDER_PWM, uint16_t PERIOD, uint16_t pwm_level);

// Função para fazer o fading
void update_pwm(int PIN, uint16_t* pwm_new_level, float indiceDeCalor);

void update_pwm2(int PIN, uint16_t* pwm_new_level);

#endif
