#include "stm32f0xx.h"
#include <stdlib.h>
#include "lcd.h"

void draw_segment(int seg, char dir, u16 c) {
    switch (seg) {
        case 1:
            if (dir == 'l') {
                // L1
                LCD_DrawFillRectangle(125, 2, 145, 4, c);
            } else {
                // R1
                LCD_DrawFillRectangle(175, 2, 195, 4, c);
            }
            break;
        case 2:
            if (dir == 'l') {
                // L2
                LCD_DrawFillRectangle(143, 2, 145, 14, c);
            } else {
                // R2
                LCD_DrawFillRectangle(193, 2, 195, 14, c);

            }
            break;
        case 3:
            if (dir == 'l') {
                // L3
                LCD_DrawFillRectangle(143, 14, 145, 28, c);
            } else {
                // R3
                LCD_DrawFillRectangle(193, 14, 195, 28, c);

            }
            break;
        case 4:
            if (dir == 'l') {
                // L4
                LCD_DrawFillRectangle(125, 26, 145, 28, c);
            } else {
                // R4
                LCD_DrawFillRectangle(175, 26, 195, 28, c);
            }
            break;
        case 5:
            if (dir == 'l') {
                // L5
                LCD_DrawFillRectangle(125, 14, 127, 28, c);
            } else {
                // R5
                LCD_DrawFillRectangle(175, 14, 177, 28, c);
            }
            break;
        case 6:
            if (dir == 'l') {
                // L6
                LCD_DrawFillRectangle(125, 2, 127, 14, c);
            } else {
                // R6
                LCD_DrawFillRectangle(175, 2, 177, 14, c);
            }
            break;
        case 7:
            if (dir == 'l') {
                // L7
                LCD_DrawFillRectangle(125, 13, 145, 15, c);
            } else {
                // R7
                LCD_DrawFillRectangle(175, 13, 195, 15, c);
            }
            break;
        default:
            LCD_DrawFillRectangle(155, 14, 165, 17, RED);
            break;
        }
}

void draw_digit(int digit, char dir, u16 c) {
    switch (digit) {
    case 1:
            draw_segment(2, dir, c);
            draw_segment(3, dir, c);
            break;
    case 2:
            draw_segment(1, dir, c);
            draw_segment(2, dir, c);
            draw_segment(4, dir, c);
            draw_segment(5, dir, c);
            draw_segment(7, dir, c);

            break;
    case 3:
            draw_segment(1, dir, c);
            draw_segment(2, dir, c);
            draw_segment(3, dir, c);
            draw_segment(4, dir, c);
            draw_segment(7, dir, c);
            break;
    case 4:
            draw_segment(2, dir, c);
            draw_segment(3, dir, c);
            draw_segment(6, dir, c);
            draw_segment(7, dir, c);
            break;
    case 5:
            draw_segment(1, dir, c);
            draw_segment(3, dir, c);
            draw_segment(4, dir, c);
            draw_segment(6, dir, c);
            draw_segment(7, dir, c);
            break;
    case 8:
            draw_segment(1, dir, c);
            draw_segment(2, dir, c);
            draw_segment(3, dir, c);
            draw_segment(4, dir, c);
            draw_segment(5, dir, c);
            draw_segment(6, dir, c);
            draw_segment(7, dir, c);
            break;
    default:
        draw_segment(1, dir, c);
        draw_segment(2, dir, c);
        draw_segment(3, dir, c);
        draw_segment(4, dir, c);
        draw_segment(5, dir, c);
        draw_segment(6, dir, c);
        break;
    }
}

void init_scoreboard() {
    LCD_DrawFillRectangle(155, 14, 165, 17, RED); // -

    draw_digit(0, 'l', RED);
    draw_digit(0, 'r', RED);
}

void overwrite_scoreboard(char dir) {
    draw_digit(8, dir, BLACK);
}
