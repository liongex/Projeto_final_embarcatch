/*
    Atividade 7 - Embarcatech
    Aluno: Isac de Lima Feliciano;
    Matrícula: 202421511720213.
    
    BIBLIOTECA RESPONSÁVEL PELA ESCRITA NO DISPLAY.

*/

#ifndef _inc_display
#define _inc_display

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

void print_texto(int x, int y, int tam, char * msg, ssd1306_t *disp); // função para escrever textos

void print_retangulo(int x1, int y1, int x2, int y2, ssd1306_t *disp); // função para desenhar retangulo do menu

void print_menu(int pos,ssd1306_t *disp); // função para desenhar o menu

void print_menu2(int pos, ssd1306_t *disp, uint16_t pwm_new_level);// função para desenhar o segundo menu

void print_menu3(int pos, ssd1306_t *disp, uint16_t pwm_new_level, float temperatura, float umidade, char conf[]);
#endif
