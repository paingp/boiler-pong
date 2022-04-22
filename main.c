#include "stm32f0xx.h"
#include <stdlib.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"

extern const Picture background; // A 240x320 background image

void init_lcd_spi(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(3<<(5*2)) & ~(3<<(5*2)) & ~(3<<(6*2)) & ~(3<<(7*2));
    GPIOC->MODER |= (1<<(4*2)) | (1<<(5*2)) | (1<<(6*2)) | (1<<(7*2));
    GPIOC->OTYPER |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
    GPIOC->MODER &= ~(3<<(0*2)) & ~(3<<(1*2)) & ~(3<<(2*2)) & ~(3<<(3*2));
    GPIOC->PUPDR &= ~(3<<(0*2)) & ~(3<<(1*2)) & ~(3<<(2*2)) & ~(3<<(3*2));
    GPIOC->PUPDR |= (1<<(0*2)) | (1<<(1*2)) | (1<<(2*2)) | (1<<(3*2));
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

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR2 = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void basic_drawing(void);
void move_ball(void);

#define VOICES 15

int32_t input[4];

struct {
    uint8_t in_use;
    uint8_t note;
    uint8_t chan;
    uint8_t volume;
    int     step;
    int     offset;
} voice[VOICES];


void note_off(int time, int chan, int key, int velo)
{
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use && voice[n].note == key) {
            voice[n].in_use = 0; // disable it first...
            voice[n].chan = 0;   // ...then clear its values
            voice[n].note = key;
            voice[n].step = step[key];
            return;
        }
    }
}

void note_on(int time, int chan, int key, int velo)
{
    if (velo == 0) {
        note_off(time, chan, key, velo);
        return;
    }
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use == 0) {
            voice[n].note = key;
            voice[n].step = step[key];
            voice[n].offset = 0;
            voice[n].volume = velo;
            voice[n].chan = chan;
            voice[n].in_use = 1;
            return;
        }
    }
}

void set_tempo(int time, int value, const MIDI_Header *hdr)
{
    TIM2->ARR = value/hdr->divisions - 1;
}

const float pitch_array[] = {
0.943874, 0.945580, 0.947288, 0.948999, 0.950714, 0.952432, 0.954152, 0.955876,
0.957603, 0.959333, 0.961067, 0.962803, 0.964542, 0.966285, 0.968031, 0.969780,
0.971532, 0.973287, 0.975046, 0.976807, 0.978572, 0.980340, 0.982111, 0.983886,
0.985663, 0.987444, 0.989228, 0.991015, 0.992806, 0.994599, 0.996396, 0.998197,
1.000000, 1.001807, 1.003617, 1.005430, 1.007246, 1.009066, 1.010889, 1.012716,
1.014545, 1.016378, 1.018215, 1.020054, 1.021897, 1.023743, 1.025593, 1.027446,
1.029302, 1.031162, 1.033025, 1.034891, 1.036761, 1.038634, 1.040511, 1.042390,
1.044274, 1.046160, 1.048051, 1.049944, 1.051841, 1.053741, 1.055645, 1.057552,
};

void pitch_wheel_change(int time, int chan, int value)
{
    //float multiplier = pow(STEP1, (value - 8192.0) / 8192.0);
    float multiplier = pitch_array[value >> 8];
    for(int n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use && voice[n].chan == chan) {
            voice[n].step = step[voice[n].note] * multiplier;
        }
    }
}

int main(void)
{
    init_wavetable_hybrid2();
    init_dac();
    init_tim6();
    MIDI_Player *mp = midi_init(midifile);
    init_tim2(10417);
    setup_buttons();
    LCD_Setup(); // this will call init_lcd_spi()
    //basic_drawing();
    LCD_DrawPicture(0,0,&background);
    move_ball();
}
