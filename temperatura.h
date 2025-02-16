/*
    Atividade 7 - Embarcatech
    Aluno: Isac de Lima Feliciano;
    Matrícula: 202421511720213.
    
    BIBLIOTECA RESPONSÁVEL PELO CÁLCULO DE VALORES DE TEMPERATURA E ÍNDICE DE CALOR.

*/

#ifndef _inc_temperatura
#define _inc_temperatura

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"


char* determinarConfortoTermico(float indiceDeCalor); //Função para determinar om conforto térmico

float celsius_to_fahrenheit(float temperature); //Função para conversão de graus celsius para fahrenheit

float calcularIndiceDeCalor(float temperature_c, float humidity);  //Função para calcualr o índice de calor

void Atualizar_cor( float indiceDeCalor); //função para atualizar a cor do led de acordo com o indice de calor

#endif
