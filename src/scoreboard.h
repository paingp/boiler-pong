/*
 * scoreboard.h
 *
 *  Created on: Apr 28, 2022
 *      Author: paing
 */

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

void draw_segment(int seg, char dir, u16 c);

void draw_digit(int digit, char dir, u16 c);

void init_scoreboard();

void overwrite_scoreboard();

#endif /* SCOREBOARD_H_ */
