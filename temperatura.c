/*
    Atividade 7 - Embarcatech
    Aluno: Isac de Lima Feliciano;
    Matrícula: 202421511720213.
    
    BIBLIOTECA RESPONSÁVEL PELO CÁLCULO DE VALORES DE TEMPERATURA E ÍNDICE DE CALOR.

*/


#include <stdio.h>
#include "pico/stdlib.h"
#include "temperatura.h"



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
float celsius_to_fahrenheit(float temperature) {
    return temperature * (9.0f / 5) + 32;
 };

 //Função para calcualr o índice de calor
 float calcularIndiceDeCalor(float temperature_c, float humidity) { 
    return -8.784695 + 1.61139411 * temperature_c + 2.338549 * humidity - 0.14611605 * temperature_c * humidity
    - 0.012308094 * (temperature_c * temperature_c) - 0.016424828 * (humidity * humidity)
     + 0.002211732 * (temperature_c * temperature_c) * humidity + 0.00072546 * temperature_c * (humidity * humidity) 
     - 0.000003582 * (temperature_c * temperature_c) * (humidity * humidity);
     };

//função para atualizar a cor do led de acordo com o indice de calor
void Atualizar_cor( float indiceDeCalor){

    if (indiceDeCalor < 28.0) { //"CONFORTAVEL"; 
        gpio_put(12,true );
        gpio_put(13,false );
        gpio_put(11,false );
    } else if (indiceDeCalor < 33.0) { // "MOD. DESCONF."
        gpio_put(12,false);
        gpio_put(13,false );
        gpio_put(11,true );     
    } else if (indiceDeCalor < 38.0) {  // "DESCONFORTAVEL"; 
        gpio_put(12,false);
        gpio_put(13,true );
        gpio_put(11,true);   
    } else if (indiceDeCalor < 45.0) {  //"PERIGOSO"
        gpio_put(12,false);
        gpio_put(13,true );
        gpio_put(11,false);   
    } else {               //  "EXTR. PERIGOSO"; 
        gpio_put(12,false);
        gpio_put(13,true );
        gpio_put(11,false);  
    }
};

