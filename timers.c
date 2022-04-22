#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"

#define VOICES 15

uint32_t input[4];
struct {
    uint8_t in_use;
    uint8_t note;
    uint8_t chan;
    uint8_t volume;
    int     step;
    int     offset;
} voice[VOICES];

//struct {
//    uint8_t note;
//    uint8_t volume;
//    int     offset;
//} sfx[VOICES]
//      {0x34234, 1, 3123}, {0x34234, 1, 3123};

void init_dac(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR &= ~DAC_CR_TSEL1;
    DAC->CR |= DAC_CR_TEN1;
    DAC->CR |= DAC_CR_EN1;
}

void TIM6_DAC_IRQHandler(void)
{
    // TODO: Remember to acknowledge the interrupt right here.
    TIM6->SR &= ~TIM_SR_UIF;
    int sample = 0;
    for(int x=0; x < sizeof voice / sizeof voice[0]; x++) {
        if (voice[x].in_use) {
            voice[x].offset += voice[x].step;
            if (voice[x].offset >= N<<16)
                voice[x].offset -= N<<16;
            sample += (wavetable[voice[x].offset>>16] * voice[x].volume) >> 4;
        }
    }
    //if(check passes)
    // check and play sound
    // sfx_sample += (wavetable[voice[x].offset>>16] * voice[x].volume) >> 4;
    //sample += sfx_sample;
    sample = (sample >> 10) + 2048;
    if (sample > 4095)
        sample = 4095;
    else if (sample < 0)
        sample = 0;
    DAC->DHR12R1 = sample;
}

void init_tim6(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 1200 - 1;
    TIM6->ARR = 2 - 1;
    TIM6->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = (1<<TIM6_DAC_IRQn);
    TIM6->CR2 &= ~TIM_CR2_MMS;
    TIM6->CR2 |= TIM_CR2_MMS_1;
    TIM6->CR1 |= TIM_CR1_CEN;
}


void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~(TIM_SR_UIF);
    midi_play();
}

void init_tim2(int n) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 48-1;
    TIM2->ARR = n-1;
    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= (1<<TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 3);
    TIM2->CR1 |= TIM_CR1_CEN;
}

extern const Picture background; // A 240x320 background image
extern const Picture ball; // A 19x19 purple ball with white boundaries
extern const Picture paddle; // A 59x5 paddle

const int border = 20;
int xmin; // Farthest to the left the center of the ball can go
int xmax; // Farthest to the right the center of the ball can go
int ymin; // Farthest to the top the center of the ball can go
int ymax; // Farthest to the bottom the center of the ball can go
int x,y; // Center of ball
int vx,vy; // Velocity components of ball

int px; // Center of paddle offset
int newpx; // New center of paddle


#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }

TempPicturePtr(object,29,29);
