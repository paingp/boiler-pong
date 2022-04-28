/*
 * game.c
 *
 *  Created on: Apr 27, 2022
 *      Author: paing
 */

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "game.h"
#include "scoreboard.h"

#include "joystick.h"

TempPicturePtr(object,29,29);

struct Ball {
    int16_t x;
    int16_t y;
    uint16_t xmin;
    uint16_t xmax;
    uint16_t ymin;
    uint16_t ymax;
    int vx;
    int vy;
} ball = {160, 120, 10, 310, 42, 230, -1, 0};
//{160, 120, 10, 310, 42, 230, -1, 0};

int input[4] = {0, 0, 0, 0};

// pl: paddle of player on the left
// pr: paddle of player on the right
int32_t pl[4] = {15, 120, 0, 0};      // x, y, prev_x, prev_y
int32_t pr[4] = {305, 120, 0, 0};

const int border = 15;
const int pl_xmin = 15;
const int pl_xmax = 100;
const int pr_xmin = 220;
const int pr_xmax = 305;
const int p_ymin = 29;
const int p_ymax = 210;

int pu_x = 0;
int pu_y = 0;

int pu_xmin = 105;
int pu_xmax = 176;
int pu_ymin = 50;
int pu_ymax = 180;

uint8_t score_l = 0;
uint8_t score_r = 0;
bool game_over = false;
uint16_t count = 0;

void perturb(int *vx, int *vy)
{
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

void overlay_ball() {
    for(int i=0; i<29*29; i++) {
            object->pix2[i] = 0xffff;
        }
    pic_overlay(object,5,5,&picball,0xffff);
}

void erase_ball() {
    TempPicturePtr(tmp,29,29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, ball.x - tmp->width/2, ball.y - tmp->height/2); // Copy the background
    LCD_DrawPicture(ball.x - tmp->width/2, ball.y - tmp->height/2, tmp); // Re-draw it to the screen
}

void update_bpos(int16_t * xoff, int16_t * yoff) {
    int16_t check = 0;
    ball.x += ball.vx;
    ball.y += ball.vy;
    if (ball.x <= ball.xmin) {
        // Ball hits left wall
        ball.vx = 0;
        ball.vy = 0;
        reset_map(-1);
        /*
        ball.vx = - ball.vx;
        if (ball.x < ball.xmin)
            ball.x += ball.vx;
        perturb(&(ball.vx), &(ball.vy));
        */
    }
    /*
    if ((ball.x - 4 <= (pl[0] + ypaddle.width)) && (ball.y < (pl[1] + 29)) && (ball.y > (pl[1] - 29))) {
        ball.vx = -ball.vx;
        if (ball.x > pl_xmin)
            ball.x += ball.vx;
    }
    if ((ball.x + 10 >= (pr[0] - bpaddle.width)) && (ball.y < (pr[1] + 29)) && (ball.y > (pr[1] - 29))) {
        ball.vx = -ball.vx;
        if (ball.x < pr_xmax)
            ball.x += ball.vx;
    }
    */
    if (ball.x >= ball.xmax) {
        // Ball hits right wall
        ball.vx = 0;
        ball.vy = 0;
        reset_map(1);
        /*
        ball.vx = -ball.vx;
        if (ball.x > ball.xmax)
            ball.x += ball.vx;
        perturb(&(ball.vx), &(ball.vy));
        */
    }
    if (ball.y <= ball.ymin) {
        ball.vy = - ball.vy;
        if (ball.y < ball.ymin)
            ball.y += ball.vy;
        perturb(&(ball.vx), &(ball.vy));
        check = -1;
    }
    if (ball.y >= ball.ymax) {
        ball.vy = -ball.vy;
        if (ball.y > ball.ymax)
            ball.y += ball.vy;
        perturb(&(ball.vx), &(ball.vy));
        check = -1;
    }
/*
    if (check == 0) {
        check = hits_object(ball.x, ball.y);
        if (check) {
            if (ball.vy != 0) {
                ball.vy = -ball.vy;
                ball.y += ball.vy;
            }
            if (check == 1) {
                if (ball.x > pl_xmin) {
                    ball.x += ball.vx;
                    *xoff = 10;
                }
            }
            else {
                if (ball.x < pr_xmax) {
                    ball.x += ball.vx;
                    *xoff = -10;
                }
            }
        }
    }*/
}

void reset_map(int8_t vx) {
    // vx = 1 means left player scores
    // vx = -1 means right player scores
    if (vx == 1) {
        score_l += 1;
        overwrite_scoreboard('l');
        draw_digit(score_l, 'l', RED);
    }
    else if (vx == -1) {
        score_r += 1;
        overwrite_scoreboard('r');
        draw_digit(score_r, 'r', RED);
    }
    if ((score_l == 5) || (score_r == 5)) {
        game_over = true;
        return;
    }
    for (uint8_t i = 0; i < 4; i++) {
        nano_wait(1000000000 * i);
    }
    erase_ball();
    ball.x = 160;
    ball.y = 120;
    ball.vx = -vx;
}

int16_t hits_object(int16_t x, int16_t y) {
    //return value: 0 - no objects are hit
    //              1 - left paddle is hit
    //              2 - right paddle is hit
    //              3 - power-up is hit
    int16_t obj = 0;
    if ((ball.vx < 0) && (x - 9 <= pl[0] + 3)) {
        // ball will hit left paddle from the right
        if (y < pl[1]) {
            // center of ball is above center of left paddle
            if (y + 9 >= pl[1] - 29) {
                ball.vx = - ball.vx;
                obj = 1;
            }
        }
        if (y > pl[1]) {
            // center of ball is below center of left paddle
            if (y - 9 <= pl[1] + 29) {
                ball.vx = - ball.vx;
                obj = 1;
            }
        }
        ball.vx = - ball.vx;
    }
    if ((ball.vx > 0) && (x + 9 >= pr[0] - 3)) {
        // ball is approaching right paddle from the left
        if (y < pr[1]) {
            // center of ball is above center of right paddle
            if (y + 9 >= pr[1] - 29) {
                ball.vx = - ball.vx;
                obj = 2;
            }
        }
        if (y > pr[1]) {
            // center of ball is below center of right paddle
            if (y - 9 <= pr[1] + 29) {
                ball.vx = - ball.vx;
                obj = 2;
            }
        }
        ball.vx = - ball.vx;
    }
    return obj;
}

void change_speed(int16_t vx, int16_t vy) {
    ball.vx = vx;
    ball.vy = vy;
}
/*
void update_pl() {
    pl[2] = pl[0];
    pl[3] = pl[1];
    if (input[0] > 10) {
        pl[0] += 1;
        if (pl[0] > pl_xmax)
            pl[0] = pl[2];
    }
    if (input[0] < -10) {
        pl[0] -= 1;
        if (pl[0] < pl_xmin)
            pl[0] = pl[2];
    }
    if (input[1] > 10) {
        pl[1] += 1;
        if (pl[1] > p_ymax) {
            pl[1] = pl[3];
        }
    }
    if (input[1] < -10) {
        pl[1] -= 1;
        if (pl[1] < p_ymin)
            pl[1] = pl[3];
    }
    TempPicturePtr(tmp, 10, 61);            // 15 -> 10
    pic_subset(tmp, &background, pl[0] - tmp->width/2, pl[1] - tmp->height/2);
    pic_overlay(tmp, 1, 1, &ypaddle, 0xffff);
    LCD_DrawPicture(pl[0] - tmp->width/2, pl[1] - tmp->height/2, tmp);
}

void update_pr() {
    pr[2] = pr[0];
    pr[3] = pr[1];
    if (input[2] > 10) {
        pr[0] += 1;
        if (pr[0] > pr_xmax)
            pr[0] = pr[2];
    }
    if (input[2] < -10) {
        pr[0] -= 1;
        if (pr[0] < pr_xmin)
            pr[0] = pr[2];
    }
    if (input[3] > 10) {
        pr[1] += 1;
        if (pr[1] > p_ymax) {
            pr[1] = pr[3];
        }
    }
    if (input[3] < -10) {
        pr[1] -= 1;
        if (pr[1] < p_ymin)
            pr[1] = pr[3];
    }
    TempPicturePtr(tmp, 7, 61);            // 15 -> 10
    pic_subset(tmp, &background, pr[0] - tmp->width/2, pr[1] - tmp->height/2);
    pic_overlay(tmp, 1, 1, &bpaddle, 0xffff);
    LCD_DrawPicture(pr[0] - tmp->width/2, pr[1] - tmp->height/2, tmp);
}

*/

void update_pl() {
    if (input[0] > 10) {
        pl[0] += 1;
        if (pl[0] > pl_xmax)
            pl[0] = pl[2];
    }
    if (input[0] < -10) {
        pl[0] -= 1;
        if (pl[0] < pl_xmin)
            pl[0] = pl[2];
    }
    if (input[1] > 10) {
        pl[1] += 1;
        if (pl[1] > p_ymax) {
            pl[1] = pl[3];
        }
    }
    if (input[1] < -10) {
        pl[1] -= 1;
        if (pl[1] < p_ymin)
            pl[1] = pl[3];
    }
    //if ((pl[0] != pl[2]) || (pl[1] != pl[3])) {
    TempPicturePtr(tmp, 7, 61);            // 15 -> 10
    pic_subset(tmp, &background, pl[0] - tmp->width/2, pl[1] - tmp->height/2);
    pic_overlay(tmp, 1, 1, &ypaddle, 0xffff);
    LCD_DrawPicture(pl[0] - tmp->width/2, pl[1] - tmp->height/2, tmp);
    //}
    pl[2] = pl[0];
    pl[3] = pl[1];
}

void update_pr() {
    if (input[2] > 10) {
        pr[0] += 1;
        if (pr[0] > pr_xmax)
            pr[0] = pr[2];
    }
    if (input[2] < -10) {
        pr[0] -= 1;
        if (pr[0] < pr_xmin)
            pr[0] = pr[2];
    }
    if (input[3] > 10) {
        pr[1] += 1;
        if (pr[1] > p_ymax) {
            pr[1] = pr[3];
        }
    }
    if (input[3] < -10) {
        pr[1] -= 1;
        if (pr[1] < p_ymin)
            pr[1] = pr[3];
    }
    //if ((pr[0] != pr[2]) || (pr[1] != pr[3])) {
    TempPicturePtr(tmp, 7, 61);            // 15 -> 10
    pic_subset(tmp, &background, pr[0] - tmp->width/2, pr[1] - tmp->height/2);
    pic_overlay(tmp, 1, 1, &bpaddle, 0xffff);
    LCD_DrawPicture(pr[0] - tmp->width/2, pr[1] - tmp->height/2, tmp);
    //}
    pr[2] = pr[0];
    pr[3] = pr[1];
}

void draw_powerup(uint8_t count) {
    pu_x = rangeRandom(pu_xmin, pu_xmax);
    pu_y = rangeRandom(pu_ymin, pu_ymax);
    if ((count % 5) == 0) {
        LCD_DrawPicture(pu_x, pu_y, &speedup);
    }
    else {
        LCD_DrawPicture(pu_x, pu_y, &reflect);
    }
}

int rangeRandom (int min, int max)
{
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do
    {
        x = rand();
    } while (x >= RAND_MAX - remainder);
    return min + x % n;
}

void setup_tim17() {
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
    TIM17->PSC = 2000-1;
    TIM17->ARR = 24-1;
    TIM17->DIER |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM17_IRQn, 2);
    NVIC->ISER[0] = (1<<TIM17_IRQn);
    TIM17->CR1 |= TIM_CR1_CEN;
}

void TIM17_IRQHandler(void)
{
    if (game_over == false) {
    TIM17->SR &= ~TIM_SR_UIF;
    //LCD_DrawPicture(105, 180, &speedup);
    //LCD_DrawPicture(176, 50, &reflect);
    check_key(input);
    update_pl();
    update_pr();
    int16_t xoff = 0;
    int16_t yoff = 0;
    update_bpos(&xoff, &yoff);
    TempPicturePtr(tmp,29,29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, ball.x - tmp->width/2, ball.y - tmp->height/2); // Copy the background
    pic_overlay(tmp, 0, 0, object, 0xffff); // Overlay the object - Ball
    //pic_overlay(tmp, -1, 1, bpaddle, 0xffff);
    //pic_overlay(tmp, (ball.x + tmp->width / 2) + bpaddle.width / 2, pr[1], &bpaddle);
    LCD_DrawPicture(ball.x - tmp->width/2, ball.y - tmp->height/2, tmp); // Re-draw it to the screen
    count++;
    /*
    if (count % 1400 == 0) {
        count = 0;
        draw_powerup(rangeRandom(0, 50));
    }
    */
    }
}

void update_screen() {
    //check_key(input);
    //update_pl();
    //update_pr();
    int16_t xoff = 0;
    int16_t yoff = 0;
    update_bpos(&xoff, &yoff);
    TempPicturePtr(tmp,29,29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, ball.x - tmp->width/2, ball.y - tmp->height/2); // Copy the background
    pic_overlay(tmp, 0, 0, object, 0xffff); // Overlay the object - Ball
    LCD_DrawPicture(ball.x - tmp->width/2, ball.y - tmp->height/2, tmp); // Re-draw it to the screen
}
