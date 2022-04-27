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

extern const Picture background;

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

int main(void)
{
    overlay_ball();
    setup_buttons();
    LCD_Setup();
    setup_joystick();
    LCD_DrawPicture(0,0, &background);
    setup_tim17();
    /*
    for(;;) {
        update_screen();
    }
    */
}
