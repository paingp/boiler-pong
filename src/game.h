/*
 * game.h
 *
 *  Created on: Apr 27, 2022
 *      Author: paing
 */

#ifndef GAME_H_
#define GAME_H_

#include "lcd.h"

#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }
/*
typedef struct {
    int16_t x;
    int16_t y;
    uint16_t xmin;
    uint16_t xmax;
    uint16_t ymin;
    uint16_t ymax;
    int vx;
    int vy;
} Ball;
*/
extern const Picture background;
extern const Picture picball;
extern const Picture ypaddle;
extern const Picture bpaddle;

extern const Picture speedup;
extern const Picture reflect;

extern const Picture win1;

void perturb(int *vx, int *vy);

void overlay_ball();

void erase_ball();

void update_bpos();

void reset_map(int8_t vx);

int16_t hits_object(int16_t nextx, int16_t nexty);

void change_speed(int16_t vx, int16_t vy);

void update_pl();

int rangeRandom (int min, int max);

void setup_tim17();

void update_screen();

#endif /* GAME_H_ */
