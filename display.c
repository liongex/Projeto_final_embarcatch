/*
    Atividade 7 - Embarcatech
    Aluno: Isac de Lima Feliciano;
    Matrícula: 202421511720213.
    
    BIBLIOTECA RESPONSÁVEL PELA ESCRITA NO DISPLAY.

*/


#include <stdio.h>
#include "pico/stdlib.h"
#include "display.h"



// função para escrever textos
void print_texto(int x, int y, int tam, char * msg, ssd1306_t *disp){
    ssd1306_draw_string(disp, x, y, tam, msg);
    ssd1306_show(disp);
};

// função para desenhar retangulo do menu
void print_retangulo(int x1, int y1, int x2, int y2, ssd1306_t *disp){
    ssd1306_draw_empty_square(disp, x1, y1, x2, y2);
    ssd1306_show(disp);
};

 // função para desenhar o menu
void print_menu(int pos,ssd1306_t *disp){
        ssd1306_clear(disp);//Limpa a tela
        print_texto(0, 2, 1, "Controle Temperatura", disp);
        print_retangulo(2,pos+2,125,12, disp);// print_retangulo(2,pos+2,120,12);
        print_texto(6, 18, 1,"Manual", disp);
        print_texto(6, 30, 1, "Automatico", disp);

};

// função para desenhar o segundo menu
void print_menu2(int pos, ssd1306_t *disp, uint16_t pwm_new_level){
    char buffer[50];
    float duty = (((float)pwm_new_level)/999);
    ssd1306_clear(disp);//Limpa a tela
    print_texto(0, 2, 1, "Controle Manual", disp);
    sprintf(buffer, "Velocidade: %.2f%%", duty*100);
    print_texto(6, 18, 1,buffer, disp);
   
};

// função para desenhar o terceiro menu
void print_menu3(int pos, ssd1306_t *disp, uint16_t pwm_new_level, float temperatura, float umidade, char conf[]){
    char buffer[50];
    float duty = (((float)pwm_new_level)/999);
    ssd1306_clear(disp);//Limpa a tela
    print_texto(0, 2, 1, "Controle Automatico", disp);
    sprintf(buffer, "Temperatura: %.1fC", temperatura);
    print_texto(0, 18, 1,buffer, disp);
    sprintf(buffer, "Umidade: %.1f%%", umidade);
    print_texto(0, 30, 1,buffer, disp);
    print_texto(0, 42, 1,conf, disp);
    sprintf(buffer, "Velocidade: %.1f%%", duty*100);
    print_texto(0, 54, 1,buffer, disp);
   
};