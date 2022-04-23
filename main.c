#include "stm32f0xx.h"
#include <stdlib.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"
int32_t input[4];
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

void setup_tim17()
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
        TIM17->PSC = 2000-1;
        TIM17->ARR = 24-1;
        TIM17->DIER |= TIM_DIER_UIE;
        NVIC_SetPriority(TIM17_IRQn, 2);
        NVIC->ISER[0] = (1<<TIM17_IRQn);

        TIM17->CR1 |= TIM_CR1_CEN;
}
int count = 0;
int newpy;
int py;
void TIM17_IRQHandler(void)
{
    TIM17->SR &= ~TIM_SR_UIF;
    check_key(input);

    if(count == 0) {
        input[0] = 0;
        input[1] = 0;
        input[2] = 0;
        input[3] = 0;
        count++;
    }
/*
    if (input[0] < -10 && input[1] < -10) { //left x and down y
        newpy -= 1; //down
        newpx -= 1; //left
    }
    else if(input[0] > 10 && input[1] > 10) { //right and up
        newpx += 1; //right
        newpy += 1; // up
    }
    else if(input[0] > 10 && input[1] < -10) { //right and down
           newpx += 1; //right
           newpy -= 1; //down
       }
    else if(input[0] < -10 && input[1] > 10) { //left and up
           newpx -= 1;
           newpy += 1;
       }*/
    if(input[0] > 10 &&  -10 > input[1] < 10) {
           newpx += 1; //right
       }
    if(input[0] < -10 &&  -10 > input[1] < 10) {
            newpx -= 1; //left
        }
    if(input[1] < -10 &&  -10 > input[0] < 10) {
            newpy -= 1; //down
        }
    if(input[1] > 10 &&  -10 > input[0] < 10) {
            newpy += 1; //up
        }
    if (newpx - paddle.width/2 <= border || newpx + paddle.width/2 >= 240-border) {
        newpx = px;
    }
    if (newpy - paddle.height/2 <= border || newpy + paddle.height/2 >= 320-border)
        newpy = py;

    if (newpx != px) {
        px = newpx;
        TempPicturePtr(tmp,61,5);
        pic_subset(tmp, &background, px - tmp->width/2, tmp->height/2); // Copy the background
        pic_overlay(tmp, 1, 0, &paddle, -1);
        LCD_DrawPicture(px - tmp->width/2, background.height - border - tmp->height, tmp);
    }
    if (newpy != py) {
        py = newpy;
        TempPicturePtr(tmp,61,5);
        pic_subset(tmp, &background, tmp->width/2, py);
        pic_overlay(tmp, 0,1, &paddle, -1);
        LCD_DrawPicture(background.width - border - tmp->width/2, py ,   tmp);
    }
    /*else if(newpy != py) {
        py = newpy;
        TempPicturePtr(tmp,61,5);
               pic_subset(tmp, &background, background.width-border-tmp->width, py-tmp->height/2 ); // Copy the background
               pic_overlay(tmp, 0, 1, &paddle, -1);
               LCD_DrawPicture( background.width-border-tmp->width,py-tmp->height/2 , tmp);
    }*/

    x += vx;
    y += vy;
    if (x <= xmin) {
        vx = - vx;
        if (x < xmin)
            x += vx;
        perturb(&vx,&vy);
    }
    if (x >= xmax) {
        // Ball hit the right wall.
        vx = -vx;
        if (x > xmax)
            x += vx;
        perturb(&vx,&vy);
    }
    if (y <= ymin) {
        // Ball hit the top wall.
        vy = - vy;
        if (y < ymin)
            y += vy;
        perturb(&vx,&vy);
    }
    if (y >= ymax - paddle.height &&
        x >= (px - paddle.width/2) &&
        x <= (px + paddle.width/2)) {
        // The ball has hit the paddle.  Bounce.
        int pmax = ymax - paddle.height;
        vy = -vy;
        if (y > pmax)
            y += vy;
    }
    else if (y >= ymax) {
        // The ball has hit the bottom wall.  Set velocity of ball to 0,0.
        vx = 0;
        vy = 0;
    }

    TempPicturePtr(tmp,29,29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
    pic_overlay(tmp,0,0, object, 0xffff); // Overlay the object - Ball
    pic_overlay(tmp, (px-paddle.width/2) - (x-tmp->width/2),
            (background.height-border-paddle.height) - (y-tmp->height/2),
            &paddle, 0xffff); // Draw the paddle into the image

    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Re-draw it to the screen
}


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

void init_lcd() {
    // Set all pixels in the object to white.
    for(int i=0; i<29*29; i++)
        object->pix2[i] = 0xffff;

    pic_overlay(object,5,5,&ball,0xffff);
    xmin = border + ball.width/2;
    xmax = background.width - border - ball.width/2;
    ymin = border + ball.width/2;
    ymax = background.height - border - ball.height/2;
    x = (xmin+xmax)/2; // Center of ball
    y = ymin;
    vx = 0; // Velocity components of ball
    vy = 1;

    px = -1; // Center of paddle offset (invalid initial value to force update)
    py = -1;
    newpx = (xmax+xmin)/2; // New center of paddle
    newpy = -1;
}

int main(void)
{
    init_lcd();
    init_wavetable_hybrid2();
    init_dac();
    init_tim6();
    MIDI_Player *mp = midi_init(midifile);
    init_tim2(10417);
    setup_buttons();
    LCD_Setup(); // this will call init_lcd_spi()
    setup_joystick();
    //basic_drawing();
    LCD_DrawPicture(0,0,&background);

    setup_tim17();

}
