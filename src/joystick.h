/*
 * joystick.h
 *
 *  Created on: Apr 26, 2022
 *      Author: pkhant
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

void setup_joystick();

int read_js1_x();

int read_js1_y();

int read_js2_x();

int read_js2_y();

void check_key(int * arr);

#endif /* JOYSTICK_H_ */
