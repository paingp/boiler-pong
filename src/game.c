#include "stm32f0xx.h"
#include <stdint.h>
#include "lcd.h"
#include "game.h"
#include "joystick.h"

TempPicturePtr(object,29,29);

struct Ball {
    int16_t x;
    int16_t y;
    uint16_t xmin;
    uint16_t xmax;
    uint16_t ymin;
    uint16_t ymax;
    int32_t vx;
    int32_t vy;
} ball = {160, 120, 10, 310, 10, 230, -1, 0};

int32_t input[4] = {0, 0, 0, 0};

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
extern int player1;
extern int player2;
extern int reset;

void perturb(int *vx, int *vy)
{
    if (*vx > 0) {
        *vx += (random()%3) - 1;
        if (*vx >= 3)
            *vx = 2;
        if (*vx == 0)
            *vx = 1;
    } else {
        *vx += (random()%3) - 1;
        if (*vx <= -3)
            *vx = -2;
        if (*vx == 0)
            *vx = -1;
    }
    if (*vy > 0) {
        *vy += (random()%3) - 1;
        if (*vy >= 3)
            *vy = 2;
        if (*vy == 0)
            *vy = 1;
    } else {
        *vy += (random()%3) - 1;
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

void update_bpos() {
    ball.x += ball.vx;
    ball.y += ball.vy;
    if (ball.x <= ball.xmin) {
        // Ball hits left wall
        /*
        ball.vx = - ball.vx;
        if (ball.x < ball.xmin)
            ball.x += ball.vx;
        perturb(&(ball.vx), &(ball.vy));
        */
        ball.vx = 0;
        ball.vy = 0;
        player1++;
        reset = 1;

    }
    if ((ball.x - 4 <= (pl[0] + ypaddle.width)) && (ball.y < (pl[1] + 29)) && (ball.y > (pl[1] - 29))) {
        ball.vx = -ball.vx;
        if (ball.x > pl_xmin)
            ball.x += ball.vx;
    }
    if ((ball.x + 4 >= (pr[0] - bpaddle.width)) && (ball.y < (pr[1] + 29)) && (ball.y > (pr[1] - 29))) {
        ball.vx = -ball.vx;
        if (ball.x > pr_xmax)
            ball.x += ball.vx;
    }
    if (ball.x >= ball.xmax) {
        // Ball hits right wall
        /*
        ball.vx = -ball.vx;
        if (ball.x > ball.xmax)
            ball.x += ball.vx;
        perturb(&(ball.vx), &(ball.vy));
         */
        ball.vx = 0;
        ball.vy = 0;
        player2++;
        reset = 1;
    }
    if (ball.y <= ball.ymin) {
        ball.vy = - ball.vy;
        if (ball.y < ball.ymin)
            ball.y += ball.vy;
        perturb(&(ball.vx), &(ball.vy));
    }
    if (ball.y >= ball.ymax) {
        ball.vy = -ball.vy;
        if (ball.y > ball.ymax)
            ball.y += ball.vy;
        perturb(&(ball.vx), &(ball.vy));
    }
}

void change_speed(int16_t vx, int16_t vy) {
    ball.vx = vx;
    ball.vy = vy;
}

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

void init_screen() {
    pl[0] = 15;
    pl[1] = 120;
    pl[2] = 0;
    pl[3] = 0;
    pr[0] = 305;
    pr[1] = 120;
    pr[2] = 0;
    pr[3] = 0;

    ball.x = 160;
    ball.y = 120;
    ball.xmin = 10;
    ball.xmax = 310;
    ball.ymin = 10;
    ball.ymax = 230;
    ball.vx = -1;
    ball.vy = 0;

    TempPicturePtr(tmp, 10, 61);            // 15 -> 10
    pic_subset(tmp, &background, pl[0] - tmp->width / 2, pl[1] - tmp->height / 2);
    pic_overlay(tmp, 1, 1, &ypaddle, 0xffff);
    LCD_DrawPicture(pl[0] - tmp->width / 2, pl[1] - tmp->height / 2, tmp);

    TempPicturePtr(tmp1, 10, 61);            // 15 -> 10
    pic_subset(tmp1, &background, pr[0] - tmp1->width / 2,
            pr[1] - tmp1->height / 2);
    pic_overlay(tmp1, 1, 1, &bpaddle, 0xffff);
    LCD_DrawPicture(pr[0] - tmp1->width / 2, pr[1] - tmp1->height / 2, tmp1);

    TempPicturePtr(tmp2, 29, 29); // Create a temporary 29x29 image.
    pic_subset(tmp2, &background, ball.x - tmp2->width / 2,
            ball.y - tmp2->height / 2); // Copy the background
    pic_overlay(tmp2, 1, 1, object, 0xffff); // Overlay the object - Ball

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
    TempPicturePtr(tmp, 10, 61);            // 15 -> 10
    pic_subset(tmp, &background, pr[0] - tmp->width/2, pr[1] - tmp->height/2);
    pic_overlay(tmp, 1, 1, &bpaddle, 0xffff);
    LCD_DrawPicture(pr[0] - tmp->width/2, pr[1] - tmp->height/2, tmp);
}

void update_screen() {
    TIM17->SR &= ~TIM_SR_UIF;
    check_key(input);
    update_bpos();
    update_pl();
    update_pr();

    TempPicturePtr(tmp, 29, 29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, ball.x - tmp->width / 2, ball.y - tmp->height / 2); // Copy the background
    pic_overlay(tmp, 1, 1, object, 0xffff); // Overlay the object - Ball
    /*
     pic_overlay(tmp, (pl[0] - ypaddle.width/2) - (pl[1] - tmp->width/2),
     (background.height - border - ypaddle.height) - (ball.y - tmp->height/2),
     &ypaddle, 0xffff); // Draw the paddle into the image
     pic_overlay(tmp, (pr[0] - bpaddle.width/2) - (pr[1] - tmp->width/2),
     (background.height - border - bpaddle.height) - (ball.y - tmp->height/2),
     &bpaddle, 0xffff); // Draw the paddle into the image EDITED ONE
     */
    LCD_DrawPicture(ball.x - tmp->width/2, ball.y - tmp->height/2, tmp); // Re-draw it to the screen
}

