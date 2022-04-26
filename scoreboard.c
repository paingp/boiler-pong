#include "stm32f0xx.h"
#include <stdlib.h>
#include "lcd.h"

void draw_digit(int digit, char dir, u16 c) {
    switch (digit) {
    case 1:
        if (dir == 'l') {
            // L1
            LCD_DrawFillRectangle(2, 179, 28, 181, c);
        } else {
            // R1
            LCD_DrawFillRectangle(2, 129, 28, 131, c);
        }
        break;
    case 2:
        if (dir == 'l') {
            // L2
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(26, 170, 28, 190, c);
            LCD_DrawFillRectangle(14, 170, 16, 190, c);
            LCD_DrawFillRectangle(2, 170, 16, 172, c);
            LCD_DrawFillRectangle(14, 188, 28, 190, c);
        } else {
            // R2
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(26, 120, 28, 140, c);
            LCD_DrawFillRectangle(14, 120, 16, 140, c);
            LCD_DrawFillRectangle(2, 120, 16, 122, c);
            LCD_DrawFillRectangle(14, 138, 28, 140, c);
        }
        break;
    case 3:
        if (dir == 'l') {
            // L3
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(26, 170, 28, 190, c);
            LCD_DrawFillRectangle(14, 170, 16, 190, c);
            LCD_DrawFillRectangle(2, 170, 28, 172, c);
        } else {
            // R3
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(26, 120, 28, 140, c);
            LCD_DrawFillRectangle(14, 120, 16, 140, c);
            LCD_DrawFillRectangle(2, 120, 28, 122, c);
        }
        break;
    case 4:
        if (dir == 'l') {
            // L4
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(14, 170, 16, 190, c);
            LCD_DrawFillRectangle(2, 170, 28, 172, c);
            LCD_DrawFillRectangle(2, 188, 16, 190, c);
        } else {
            // R4
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(14, 120, 16, 140, c);
            LCD_DrawFillRectangle(2, 120, 28, 122, c);
            LCD_DrawFillRectangle(2, 138, 16, 140, c);
        }
        break;
    case 5:
        if (dir == 'l') {
            // L5
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(26, 170, 28, 190, c);

            LCD_DrawFillRectangle(14, 170, 16, 190, c);

            LCD_DrawFillRectangle(14, 170, 28, 172, c);
            LCD_DrawFillRectangle(2, 188, 16, 190, c);
        } else {
            // R5
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(26, 120, 28, 140, c);

            LCD_DrawFillRectangle(14, 120, 16, 140, c);

            LCD_DrawFillRectangle(2, 138, 16, 140, c);
            LCD_DrawFillRectangle(14, 120, 28, 122, c);
        }
        break;
    case 6:
        if (dir == 'l') {
            // L6
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(26, 170, 28, 190, c);

            LCD_DrawFillRectangle(14, 170, 16, 190, c);
            LCD_DrawFillRectangle(14, 188, 28, 190, c);

            LCD_DrawFillRectangle(14, 170, 28, 172, c);
            LCD_DrawFillRectangle(2, 188, 16, 190, c);
        } else {
            // R6
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(26, 120, 28, 140, c);

            LCD_DrawFillRectangle(14, 120, 16, 140, c);
            LCD_DrawFillRectangle(14, 138, 28, 140, c);

            LCD_DrawFillRectangle(2, 138, 16, 140, c);
            LCD_DrawFillRectangle(14, 120, 28, 122, c);
        }
        break;
    case 7:
        if (dir == 'l') {
            // L7
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(2, 170, 28, 172, c);
        } else {
            // R7
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(2, 120, 28, 122, c);
        }
        break;
    case 8:
        if (dir == 'l') {
            // L8
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(2, 188, 28, 190, c);
            LCD_DrawFillRectangle(14, 170, 16, 190, c);
            LCD_DrawFillRectangle(26, 170, 28, 190, c);
            LCD_DrawFillRectangle(2, 170, 28, 172, c);
        } else {
            // R8
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(2, 138, 28, 140, c);
            LCD_DrawFillRectangle(14, 120, 16, 140, c);
            LCD_DrawFillRectangle(26, 120, 28, 140, c);
            LCD_DrawFillRectangle(2, 120, 28, 122, c);
        }
        break;
    case 9:
        if (dir == 'l') {
            // L9
            LCD_DrawFillRectangle(2, 170, 4, 190, c);

            LCD_DrawFillRectangle(14, 170, 16, 190, c);

            LCD_DrawFillRectangle(2, 170, 28, 172, c);
            LCD_DrawFillRectangle(2, 188, 16, 190, c);
        } else {
            // R9
            LCD_DrawFillRectangle(2, 120, 4, 140, c);

            LCD_DrawFillRectangle(14, 120, 16, 140, c);

            LCD_DrawFillRectangle(2, 120, 28, 122, c);
            LCD_DrawFillRectangle(2, 138, 16, 140, c);
        }
        break;
    default:
        if (dir == 'l') {
            // L0
            LCD_DrawFillRectangle(2, 170, 4, 190, c);
            LCD_DrawFillRectangle(2, 188, 28, 190, c);
            LCD_DrawFillRectangle(26, 170, 28, 190, c);
            LCD_DrawFillRectangle(2, 170, 28, 172, c);
        } else {
            // R0
            LCD_DrawFillRectangle(2, 120, 4, 140, c);
            LCD_DrawFillRectangle(2, 138, 28, 140, c);
            LCD_DrawFillRectangle(26, 120, 28, 140, c);
            LCD_DrawFillRectangle(2, 120, 28, 122, c);
        }
        break;
    }
}

void init_scoreboard() {
    LCD_DrawFillRectangle(14, 150, 17, 160, RED); // -

    draw_digit(0, 'l', RED);
    draw_digit(0, 'r', RED);
}

void overwrite_scoreboard(char dir) {
    draw_digit(8, dir, BLACK);
}
