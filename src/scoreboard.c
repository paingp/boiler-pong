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

void disp_win(u16 c) {
    LCD_DrawFillRectangle(70, 80, 250, 180, DGREEN);

    LCD_DrawFillRectangle(80, 90, 82, 130, c);
    LCD_DrawFillRectangle(83, 90, 104, 92, c);
    LCD_DrawFillRectangle(102, 93, 104, 109, c);
    LCD_DrawFillRectangle(83, 110, 104, 112, c);

    LCD_DrawFillRectangle(115, 90, 117, 130, c);

    LCD_DrawFillRectangle(132, 110, 147, 112, c);
    LCD_DrawFillRectangle(129, 110, 131, 130, c);
    LCD_DrawFillRectangle(148, 110, 150, 130, c);
    LCD_DrawFillRectangle(132, 128, 156, 130, c);

    LCD_DrawFillRectangle(165, 105, 167, 120, c);
    LCD_DrawFillRectangle(165, 121, 183, 123, c);
    LCD_DrawFillRectangle(184, 105, 186, 138, c);
    LCD_DrawFillRectangle(165, 136, 183, 138, c);

    LCD_DrawFillRectangle(196, 110, 198, 130, c);
    LCD_DrawFillRectangle(215, 110, 217, 118, c);
    LCD_DrawFillRectangle(199, 110, 214, 112, c);
    LCD_DrawFillRectangle(199, 119, 217, 121, c);
    LCD_DrawFillRectangle(196, 130, 217, 132, c);

    LCD_DrawFillRectangle(225, 110, 227, 130, c);
    LCD_DrawFillRectangle(228, 114, 240, 116, c);

    LCD_DrawFillRectangle(132, 150, 134, 168, c);
    LCD_DrawFillRectangle(143, 150, 145, 168, c);
    LCD_DrawFillRectangle(154, 150, 156, 168, c);
    LCD_DrawFillRectangle(132, 168, 156, 170, c);

    LCD_DrawFillRectangle(168, 154, 170, 170, c);
    LCD_DrawFillRectangle(168, 144, 170, 146, c);

    LCD_DrawFillRectangle(180, 150, 182, 170, c);
    LCD_DrawFillRectangle(196, 155, 198, 170, c);
    LCD_DrawFillRectangle(183, 155, 195, 157, c);

    LCD_DrawFillRectangle(212, 150, 214, 160, c);
    LCD_DrawFillRectangle(228, 160, 230, 170, c);
    LCD_DrawFillRectangle(215, 158, 230, 160, c);
    LCD_DrawFillRectangle(215, 150, 230, 152, c);
    LCD_DrawFillRectangle(212, 168, 227, 170, c);
    if (c == 0xF81F) {
        LCD_DrawFillRectangle(102, 135, 104, 170, c);     // 1
    }
    else {
        LCD_DrawFillRectangle(88, 135, 108, 137, c);
        LCD_DrawFillRectangle(108, 135, 110, 150, c);
        LCD_DrawFillRectangle(88, 151, 110, 153, c);
        LCD_DrawFillRectangle(88, 154, 90, 167, c);
        LCD_DrawFillRectangle(88, 168, 110, 170, c);

    }
}
