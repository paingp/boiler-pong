#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"

int32_t input[4];

#define VOICES 15
int player1 = 0;
int player2 = 0;
int reset = 0;


struct {
    uint8_t in_use;
    uint8_t note;
    uint8_t chan;
    uint8_t volume;
    int     step;
    int     offset;
} voice[VOICES];

void nanowait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
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

void draw_l1(u16 c) {
    // L1
    LCD_DrawFillRectangle(2, 179, 28, 181, c);
}

void draw_r1(u16 c) {
    // R1
    LCD_DrawFillRectangle(2, 129, 28, 131, c);
}


//MIDI
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
    sample = (sample >> 10) + 2048;
    if (sample > 4095)
        sample = 4095;
    else if (sample < 0)
        sample = 0;
    DAC->DHR12R1 = sample;
}

void init_dac(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR &= ~DAC_CR_TSEL1;
    DAC->CR |= DAC_CR_TEN1;
    DAC->CR |= DAC_CR_EN1;
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
        TIM2->CR1 |= TIM_CR1_CEN;

        NVIC_SetPriority(TIM2_IRQn, 3);
}

//////////////////////

void pic_subset(Picture *dst, const Picture *src, int sx, int sy) {
    int dw = dst->width;
    int dh = dst->height;
    for (int y = 0; y < dh; y++) {
        if (y + sy < 0)
            continue;
        if (y + sy >= src->height)
            break;
        for (int x = 0; x < dw; x++) {
            if (x + sx < 0)
                continue;
            if (x + sx >= src->width)
                break;
            dst->pix2[dw * y + x] = src->pix2[src->width * (y + sy) + x + sx];
        }
    }
}

void pic_overlay(Picture *dst, int xoffset, int yoffset, const Picture *src,
        int transparent) {
    for (int y = 0; y < src->height; y++) {
        int dy = y + yoffset;
        if (dy < 0)
            continue;
        if (dy >= dst->height)
            break;
        for (int x = 0; x < src->width; x++) {
            int dx = x + xoffset;
            if (dx < 0)
                continue;
            if (dx >= dst->width)
                break;
            unsigned short int p = src->pix2[y * src->width + x];
            if (p != transparent)
                dst->pix2[dy * dst->width + dx] = p;
        }
    }
}

void perturb(int *vx, int *vy) {
    if (*vx > 0) {
        *vx += (random() % 3) - 1;
        if (*vx >= 3)
            *vx = 2;
        if (*vx == 0)
            *vx = 1;
    } else {
        *vx += (random() % 3) - 1;
        if (*vx <= -3)
            *vx = -2;
        if (*vx == 0)
            *vx = -1;
    }
    if (*vy > 0) {
        *vy += (random() % 3) - 1;
        if (*vy >= 3)
            *vy = 2;
        if (*vy == 0)
            *vy = 1;
    } else {
        *vy += (random() % 3) - 1;
        if (*vy <= -3)
            *vy = -2;
        if (*vy == 0)
            *vy = -1;
    }
}

extern const Picture background; // A 240x320 background image
extern const Picture ball; // A 19x19 purple ball with white boundaries
extern const Picture paddle; // A 59x5 paddle
extern const Picture paddle2;

const int border = 20;
int xmin; // Farthest to the left the center of the ball can go - 29
int xmax; // Farthest to the right the center of the ball can go - 211
int ymin; // Farthest to the top the center of the ball can go - 29
int ymax; // Farthest to the bottom the center of the ball can go - 291
int x, y; // Center of ball
int vx, vy; // Velocity components of ball

int px1; // Center of paddle1 offset x
int py1; // Center of paddle1 offset y
int newpx1;// New center of paddle1 x
int newpy1; // New center of paddle1 y

int px2;
int py2;
int newpx2;
int newpy2;

#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }

TempPicturePtr(object, 29, 29);

void read_js1_x() {
    ADC1->CHSELR = ADC_CHSELR_CHSEL12;
    while (!(ADC1->ISR & ADC_ISR_ADRDY))
        ;
    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC))
        ;
    input[0] = ADC1->DR;
}
void read_js1_y() {
    ADC1->CHSELR = ADC_CHSELR_CHSEL11;
    while (!(ADC1->ISR & ADC_ISR_ADRDY))
        ;
    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC))
        ;
    input[1] = ADC1->DR;
}
void read_js2_x() {
    ADC1->CHSELR = ADC_CHSELR_CHSEL2;
    while (!(ADC1->ISR & ADC_ISR_ADRDY))
        ;

    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC))
        ;
    input[3] = ADC1->DR;
}
void read_js2_y() {
    ADC1->CHSELR = ADC_CHSELR_CHSEL1;
    while (!(ADC1->ISR & ADC_ISR_ADRDY))
        ;
    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC))
        ;
    input[2] = ADC1->DR;
}

void check_key() {
    read_js1_y();
    int y_1 = input[2] - 30; //y_1
    read_js1_x();
    int x_1 = input[0] - 30; //x_1
    read_js2_y();
    int y_2 = input[3] - 30; //y_2
    read_js2_x();
    int x_2 = input[1] - 30; //x_2

    input[0] = x_1;
    input[1] = x_2;
    input[2] = y_1;
    input[3] = y_2;
}

void paddle_one() {
    if (input[0] < -10) {
        newpx1 -= 1;
    } else if (input[0] > 10) {
        newpx1 += 1;
    }
    if (input[1] < -10) {
        newpy1 -= 1;
    } else if (input[1] > 10) {
        newpy1 += 1;
    }

    if (newpx1 - paddle.width / 2 <= border
            || newpx1 + paddle.width / 2 >= 240 - border) {
        newpx1 = px1;
    }
    if (newpy1 - paddle.height / 2 <= border
            || newpy1 + paddle.height / 2 >= 320 - border) {
        newpy1 = py1;
    }
    if ((newpx1 != px1) || (newpy1 != py1)) {
        if (newpx1 != px1) {
            px1 = newpx1;
        }
        if (newpy1 != py1) {
            py1 = newpy1;
        }
    }
}

void paddle_two() {
    if (input[0] < -10) {
        newpx2 -= 1;
    } else if (input[0] > 10) {
        newpx2 += 1;
    }
    if (input[1] < -10) {
        newpy2 -= 1;
    } else if (input[1] > 10) {
        newpy2 += 1;
    }

    if (newpx2 - paddle2.width / 2 <= border || newpx2 + paddle2.width / 2 >= 240 - border) {
        newpx2 = px2;
    }
    if (newpy2 - paddle2.height / 2 <= border || newpy2 + paddle2.height / 2 >= 320 - border) {
        newpy2 = py2;
    }
    if ((newpx2 != px2) || (newpy2 != py2)) {
        if (newpx2 != px2) {
            px2 = newpx2;
        }
        if (newpy2 != py2) {
            py2 = newpy2;
        }
    }
}

void screen_position_paddles() {
    paddle_one();
    paddle_two();

        TempPicturePtr(tmp, 61, 5);
        pic_subset(tmp, &background, px1 - tmp->width / 2, py1 - tmp->height / 2); // Copy the background
        pic_subset(tmp, &background, px2 - tmp->width / 2, py2 - tmp->height / 2); // Copy the background
        pic_overlay(tmp, 1, 0, &paddle, -1);
        pic_overlay(tmp, 1, 0, &paddle2, -1);
        LCD_DrawPicture(px1 - tmp->width / 2, py1 - tmp->height, tmp);
        LCD_DrawPicture(px2 - tmp->width / 2, py2 - tmp->height, tmp);
}

void ball_vector() {
    x += vx;
    y += vy;
    if (x <= xmin) {
        vx = -vx;
        if (x < xmin)
            x += vx;
        perturb(&vx, &vy);
    }
    if (x >= xmax) {
        // Ball hit the right wall.
        vx = -vx;
        if (x > xmax)
            x += vx;
        perturb(&vx, &vy);
    }
    if (y <= ymin) {
        // Ball hit the top wall.
        vx = 0;
        vy = 0;
        player2++;
        reset = 1;
    }
    if (((y >= (py1 - paddle.height) && y <= (py1 + paddle.height) && x >= (px1 - paddle.width / 2) && x <= (px1 + paddle.width / 2)))
    || (y >= (py2 - paddle.height) && y <= (py2 + paddle.height) && x >= (px2 - paddle.width / 2) && x <= (px2 + paddle.width / 2))){
        //y >= ymax - paddle.height && x >= (px1 - paddle.width/2) && x <= (px1 + paddle.width/2)) {
        // The ball has hit the paddle.  Bounce.
        int pmax = ymax - paddle.height;
        vy = -vy;
        if (y > pmax)
            y += vy;
    } else if (y >= ymax) {
        // The ball has hit the bottom wall.  Set velocity of ball to 0,0.
        vx = 0;
        vy = 0;
        player1++;
        reset = 1;
    }
}

void screen_position_ball() {
    TempPicturePtr(tmp, 29, 29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x - tmp->width / 2, y - tmp->height / 2); // Copy the background
    pic_overlay(tmp, 0, 0, object, 0xffff); // Overlay the object
    pic_overlay(tmp, ((px1 - paddle.width / 2) - (x - tmp->width / 2)),
            ((py1 - paddle.height / 2) - (y - tmp->height / 2)), &paddle,
            0xffff);
    // Draw the paddle into the image //91 - 106

    LCD_DrawPicture(x - tmp->width / 2, y - tmp->height / 2, tmp); // Re-draw it to the screen
}

int count = 0;

void check_screen_forever(void) {
    check_key();
    if (count == 0) {
        input[0] = 0; //x 1st paddle
        input[1] = 0; //y 1st paddle
        input[2] = 0; //x 2nd paddle
        input[3] = 0; //y 2nd paddle
        count++;
    }

    screen_position_paddles();
    ball_vector();
    screen_position_ball();
}

void init_lcd_spi(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (5 * 2)) & ~(3 << (5 * 2)) & ~(3 << (6 * 2))
            & ~(3 << (7 * 2));
    GPIOC->MODER |= (1 << (4 * 2)) | (1 << (5 * 2)) | (1 << (6 * 2))
            | (1 << (7 * 2));
    GPIOC->OTYPER |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

    GPIOC->MODER &= ~(3 << (0 * 2)) & ~(3 << (1 * 2)) & ~(3 << (2 * 2))
            & ~(3 << (3 * 2));
    GPIOC->PUPDR &= ~(3 << (0 * 2)) & ~(3 << (1 * 2)) & ~(3 << (2 * 2))
            & ~(3 << (3 * 2));
    GPIOC->PUPDR |= (1 << (0 * 2)) | (1 << (1 * 2)) | (1 << (2 * 2))
            | (1 << (3 * 2));
}

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
    while (!(RCC->CR2 & RCC_CR2_HSI14RDY))
        ;
    //ADC1->CFGR1 |= ADC_CFGR1_CONT;
    // Chose 6-bit resolution
    ADC1->CFGR1 |= ADC_CFGR1_RES;
    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY))
        ;
}

void setup_buttons(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(3 << (8 * 2)) & ~(3 << (11 * 2)) & ~(3 << (14 * 2));
    GPIOB->MODER |= (1 << (8 * 2)) | (1 << (11 * 2)) | (1 << (14 * 2));
    GPIOB->ODR |= (1 << 8) | (1 << 11) | (1 << 14);

    GPIOB->MODER &= ~(3 << (3 * 2)) & ~(3 << (5 * 2));
    GPIOB->MODER |= (2 << (3 * 2)) | (2 << (5 * 2));
    GPIOB->AFR[0] &= ~(0xf << (3 * 4)) & ~(0xf << (5 * 4));

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR2 = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void init_screen_one() {
    LCD_DrawPicture(0, 0, &background);

    // Set all pixels in the object to white.
    for (int i = 0; i < 29 * 29; i++)
        object->pix2[i] = 0xffff;

    pic_overlay(object, 5, 5, &ball, 0xffff);

    xmin = border + ball.width / 2;
    xmax = background.width - border - ball.width / 2;
    ymin = border + ball.width / 2;
    ymax = background.height - border - ball.height / 2;
    x = (xmin + xmax) / 2; // Center of ball
    y = (ymin + ymax) / 2;
    vx = 0; // Velocity components of ball
    vy = 1;

    px1 = -1; // Center of paddle offset (invalid initial value to force update)
    newpx1 = (xmax + xmin) / 2; // New center of paddle
    newpy1 = ymax;

    px2 = -1;
    newpx2 = (xmax + xmin) / 2;
    newpy2 = ymin + 10;
}

int main(void) {

    switch (reset) {
    case 0:
        init_wavetable_hybrid2();
        init_dac();
        init_tim6();
        MIDI_Player *mp = midi_init(midifile);
        init_tim2(10417);

        setup_buttons();

        init_lcd_spi();
        LCD_Setup();

        // Draw the background.

        setup_joystick();
        init_screen_one();

        for (;;) {
            draw_l0(RED); //Player 1
            draw_r0(RED); //Player 2
            LCD_DrawFillRectangle(14, 150, 17, 160, RED);
            check_screen_forever();
            if (mp->nexttick == MAXTICKS) {
                mp = midi_init(midifile);
            }
            if(reset == 1) {
                TIM2->CR1 &= ~TIM_CR1_CEN;
                for(int x=0; x < sizeof voice / sizeof voice[0]; x++) {
                    voice[x].in_use = 0;
                }
                break;
            }
        }
        main();
        break;
    default:
        reset = 0;
        nanowait(5000000000);
        init_wavetable_hybrid2();
        init_dac();
        init_tim6();

        mp = midi_init(midifile);
        init_tim2(10417);

        setup_buttons();

        init_lcd_spi();
        LCD_Setup();


        // Draw the background.

        setup_joystick();
        init_screen_one();
        for (;;) {
            switch (player2) {
            case 0: draw_l0(RED); break;
            case 1: draw_l1(RED); break;
            default: draw_l0(RED); break;
            }
            switch (player1) {
            case 0: draw_r0(RED); break;
            case 1: draw_r1(RED); break;
            default: draw_r0(RED); break;
            }
            LCD_DrawFillRectangle(14, 150, 17, 160, RED);
            check_screen_forever();
            if (mp->nexttick == MAXTICKS) {
                mp = midi_init(midifile);
            }
            if (reset == 1) {
                TIM2->CR1 &= ~TIM_CR1_CEN;
                for (int x = 0; x < sizeof voice / sizeof voice[0]; x++) {
                    voice[x].in_use = 0;
                }
                break;
            }
        }
        main();
    }
}

