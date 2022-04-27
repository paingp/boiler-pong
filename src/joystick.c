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

void setup_joystick() {
    // Enable clock to GPIOC and set pc0 as input, pc1 and pc2 as analog
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
    GPIOC->MODER &= ~GPIO_MODER_MODER0;
    GPIOC->MODER |= GPIO_MODER_MODER1 | GPIO_MODER_MODER2;
    GPIOA->MODER |= GPIO_MODER_MODER1 | GPIO_MODER_MODER2;
    // Enable clock to ADC peripheral
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // Turn on high-speed internal clock
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while(!(RCC->CR2 & RCC_CR2_HSI14RDY));
    //ADC1->CFGR1 |= ADC_CFGR1_CONT;
    // Chose 6-bit resolution
    ADC1->CFGR1 |= ADC_CFGR1_RES;
    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
}

int read_js1_x() {
    ADC1->CHSELR = ADC_CHSELR_CHSEL12;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
    while(!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

int read_js1_y() {
    ADC1->CHSELR = ADC_CHSELR_CHSEL11;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
    while(!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

int read_js2_x() {
    ADC1->CHSELR = 0;
    ADC1->CHSELR = ADC_CHSELR_CHSEL1;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
    while(!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

int read_js2_y() {
    ADC1->CHSELR = 0;
    ADC1->CHSELR = ADC_CHSELR_CHSEL2;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
    while(!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

void check_key(int * arr) {
    arr[0] = read_js1_x() - 30;
    arr[1] = read_js1_y() - 30;
    arr[2] = read_js2_x() - 30;
    arr[3] = read_js2_y() - 30;
}
