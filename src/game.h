/*
 * game.h
 *
 *  Created on: Apr 26, 2022
 *      Author: pkhant
 */

#ifndef GAME_H_
#define GAME_H_

#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }

typedef struct {
    int16_t x;
    int16_t y;
    uint16_t xmin;
    uint16_t xmax;
    uint16_t ymin;
    uint16_t ymax;
    int16_t vx;
    int16_t vy;
} Ball;

extern const Picture background;
extern const Picture picball;
extern const Picture ypaddle;
extern const Picture bpaddle;

void perturb(int *vx, int *vy);

void overlay_ball();

void update_bpos();

void init_screen();

void change_speed(int16_t vx, int16_t vy);

void update_pl();

void setup_tim17();

void update_screen();

#endif /* GAME_H_ */
