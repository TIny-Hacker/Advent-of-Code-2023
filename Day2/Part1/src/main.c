#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include "asm/tools.h"

// Advance to space
char *advance(char *currentChar) {
    while (*currentChar != ' ' && *currentChar != '\0') {
        currentChar++;
    }

    return currentChar;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *currentChar = ti_GetDataPtr(slot);
    char *endOfFile = currentChar + ti_GetSize(slot);
    ti_Close(slot);

    unsigned int total = 0;
    uint8_t gameID;

    while (currentChar < endOfFile) {
        bool gameValid = true;
        currentChar += 5;
        gameID = getNumber(currentChar);

        currentChar = advance(currentChar) + 1;

        while (*currentChar != 'G' && currentChar < endOfFile) {
            uint8_t value = getNumber(currentChar);
            currentChar = advance(currentChar) + 1;

            if (*currentChar == 'r' && value > 12) {
                gameValid = false;
            } else if (*currentChar == 'g' && value > 13) {
                gameValid = false;
            } else if (*currentChar == 'b' && value > 14) {
                gameValid = false;
            }

            if (gameValid) {
                currentChar = advance(currentChar) + 1;
            } else {
                currentChar += strlen(currentChar) + 1; // Advance to next line early
            }
        }

        if (gameValid) {
            total += gameID;
        }
    }


    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
