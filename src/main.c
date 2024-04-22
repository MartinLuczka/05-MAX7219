#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
#include "daughterboard.h"
#include "max7219.h"

#define DIN_PORT GPIOB

#define DIN_PIN GPIO_PIN_2

#define CS_PORT GPIOB

#define CS_PIN GPIO_PIN_3

#define CLK_PORT GPIOB

#define CLK_PIN GPIO_PIN_4

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktování MCU na 16 MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);


    init_milis();
}

void display(uint8_t adress, uint8_t data) {
    uint8_t mask;
    LOW(CS); // začátek přenosu

    /*pošlu adresu*/
    mask = 1 << 7; // 0b10000000
    while(mask) {
        if (adress & mask) {
            HIGH(DIN);
        }
        else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1; // provede se osmkrát (jednička tam bude 8x, bude se postuně posouvat až zmizí)
        LOW(CLK);
    }

    /*pošlu data*/

    mask = 1 << 7; // 0b10000000
    while(mask) {
        if (data & mask) {
            HIGH(DIN);
        }
        else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1; // provede se osmkrát (jednička tam bude 8x, bude se postuně posouvat až zmizí)
        LOW(CLK);
    }

    HIGH(CS); // konec přenosu
}


int main(void)
{
  
    uint32_t time = 0;

    int8_t number = 0;

    init();

    display(DECODE_MODE, 0b11111111); // zapneme znakovou sadu na všech cifrách
    display(SCAN_LIMIT, 7); // chceme zapnout všechny cifry
    display(INTENSITY, 1);
    display(DISPLAY_TEST,DISPLAY_TEST_OFF);
    display(SHUTDOWN, SHUTDOWN_ON);
    display(DIGIT0, 0xF);
    display(DIGIT1, 0xF);
    display(DIGIT2, 0xF);
    display(DIGIT3, 0xF);
    display(DIGIT4, 0xF);
    display(DIGIT5, 0xF);
    display(DIGIT6, 0xF);
    display(DIGIT7, 0xF);


    while(1) {
        if(milis() - time > 333) {
            time = milis();
            display(DIGIT0, number++);
        } 
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
