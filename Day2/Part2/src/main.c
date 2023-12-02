#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include "asm/tools.h"
#include "gfx/gfx.h"

// Advance to space
char *advance(char *currentChar) {
    while (*currentChar != ' ' && *currentChar != '\0') {
        currentChar++;
    }

    return currentChar;
}

void drawStats(uint8_t red, uint8_t green, uint8_t blue, unsigned int total, bool shadow) {
    for (int x = 0; x < red; x++) {
        gfx_SetColor(2 + shadow);
        gfx_FillRectangle_NoClip(58 + shadow + 12 * x, 62 + shadow, 10, 10);
    }

    for (int x = 0; x < green; x++) {
        gfx_SetColor(4 + shadow);
        gfx_FillRectangle_NoClip(58 + shadow + 12 * x, 74 + shadow, 10, 10);
    }

    for (int x = 0; x < blue; x++) {
        gfx_SetColor(58 + shadow);
        gfx_FillRectangle_NoClip(58 + shadow + 12 * x, 86 + shadow, 10, 10);
    }

    gfx_SetTextFGColor(8 + shadow);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("-------------------------------", 52 + shadow, 104 + shadow);
    gfx_PrintStringXY("Red: ", 58 + shadow, 116 + shadow);
    gfx_PrintUInt(red, 0);
    gfx_PrintStringXY("Green: ", 58 + shadow, 128 + shadow);
    gfx_PrintUInt(green, 0);
    gfx_PrintStringXY("Blue: ", 58 + shadow, 140 + shadow);
    gfx_PrintUInt(blue, 0);
    gfx_PrintStringXY("Power: ", 58 + shadow, 152 + shadow);
    gfx_PrintUInt(red * green * blue, 0);
    gfx_PrintStringXY("Total: ", 58 + shadow, 164 + shadow);
    gfx_PrintUInt(total, 0);
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *currentChar = ti_GetDataPtr(slot);
    char *endOfFile = currentChar + ti_GetSize(slot);
    ti_Close(slot);

    unsigned int total = 0;

    gfx_Begin();
    gfx_SetPalette(palette, sizeof(palette), 0);
    gfx_SetTextBGColor(1);
    gfx_SetTextTransparentColor(1);
    gfx_ZeroScreen();
    gfx_SetDrawBuffer();

    while (currentChar < endOfFile) {
        uint8_t minRed = 0;
        uint8_t minGreen = 0;
        uint8_t minBlue = 0;
        currentChar = advance(currentChar) + 1; // Skip game and game ID
        currentChar = advance(currentChar) + 1;

        while (*currentChar != 'G' && currentChar < endOfFile) {
            uint8_t value = getNumber(currentChar);
            currentChar = advance(currentChar) + 1;

            if (*currentChar == 'r' && value > minRed) {
                minRed = value;
            } else if (*currentChar == 'g' && value > minGreen) {
                minGreen = value;
            } else if (*currentChar == 'b' && value > minBlue) {
                minBlue = value;
            }

            currentChar = advance(currentChar) + 1;

            gfx_ZeroScreen();
            drawStats(minRed, minGreen, minBlue, total, true);
            drawStats(minRed, minGreen, minBlue, total, false);
            gfx_BlitBuffer();
        }

        total += minRed * minGreen * minBlue;
    }

    gfx_SetTextFGColor(9);
    gfx_PrintStringXY("Complete!", 130, 187);
    gfx_SetTextFGColor(8);
    gfx_PrintStringXY("Complete!", 129, 186);
    gfx_BlitBuffer();
    while (!kb_AnyKey());
    gfx_End();
}
