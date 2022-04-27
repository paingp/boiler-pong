/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdlib.h>
#include "lcd.h"
#include "game.h"
#include "joystick.h"
#include "midi.h"
#include "midiplay.h"
#include "music.h"
#include "scoreboard.c"

extern const Picture background;

int player1 = 0;
int player2 = 0;
int reset = 0;

void nanowait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void init_lcd_spi();
void setup_buttons();

void init_lcd_spi(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(3<<(5*2)) & ~(3<<(5*2)) & ~(3<<(6*2)) & ~(3<<(7*2));
    GPIOC->MODER |= (1<<(4*2)) | (1<<(5*2)) | (1<<(6*2)) | (1<<(7*2));
    GPIOC->OTYPER |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
    GPIOC->MODER &= ~(3<<(0*2)) & ~(3<<(1*2)) & ~(3<<(2*2)) & ~(3<<(3*2));
    GPIOC->PUPDR &= ~(3<<(0*2)) & ~(3<<(1*2)) & ~(3<<(2*2)) & ~(3<<(3*2));
    GPIOC->PUPDR |= (1<<(0*2)) | (1<<(1*2)) | (1<<(2*2)) | (1<<(3*2));

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR2 = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void setup_buttons(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(3<<(8*2)) & ~(3<<(11*2)) & ~(3<<(14*2));
    GPIOB->MODER |= (1<<(8*2)) | (1<<(11*2)) | (1<<(14*2));
    GPIOB->ODR |= (1 << 8) | (1 << 11) | (1 << 14);

    GPIOB->MODER &= ~(3<<(3*2)) & ~(3<<(5*2));
    GPIOB->MODER |= (2 << (3*2)) | (2 << (5*2));
    GPIOB->AFR[0] &= ~(0xf<<(3*4)) & ~(0xf<<(5*4));
}

void music(void) {
    init_wavetable_hybrid2();
    init_dac();
    init_tim6();
    //MIDI_Player *mp = midi_init(midifile);
    init_tim2(10417);
}

void player_check(void) {
    switch (player1) {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
    switch (player2) {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
}

void draw_l0(u16 c) {
    // L0
    LCD_DrawFillRectangle(2, 170, 4, 190, c);
    LCD_DrawFillRectangle(2, 188, 28, 190, c);
    LCD_DrawFillRectangle(26, 170, 28, 190, c);
    LCD_DrawFillRectangle(2, 170, 28, 172, c);
}

void draw_r0(u16 c) {
    // R0
    LCD_DrawFillRectangle(2, 120, 4, 140, c);
    LCD_DrawFillRectangle(2, 138, 28, 140, c);
    LCD_DrawFillRectangle(26, 120, 28, 140, c);
    LCD_DrawFillRectangle(2, 120, 28, 122, c);
}

int main(void)
{
    MIDI_Player *mp = midi_init(midifile);
    int store = 0; //Check if it is a reset and store 0 or 1
    if(reset == 0) { music(); }
    if(reset == 1) {
        reset = 0;
        nanowait(5000000000);
        TIM2->CR1 |= TIM_CR1_CEN;
        store++;
    }
    overlay_ball();
    setup_buttons();
    LCD_Setup();
    setup_joystick();
    LCD_DrawPicture(0, 0, &background);
    //draw_digit(1, 1, RED); //Use values with player1 and player2
    if(store == 1) {
        player_check();
        init_screen();
        store = 0;
    }
    for (;;) {
        update_screen();
        if (mp->nexttick == MAXTICKS) {
            mp = midi_init(midifile);
        }
        if (reset == 1) {
            TIM2->CR1 &= ~TIM_CR1_CEN;
            music_off();
            break;
        }
    }
    main();
}
