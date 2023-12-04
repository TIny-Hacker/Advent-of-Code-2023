#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include "asm/tools.h"

// Advance to space
char *advance(char *currentChar, uint8_t advanceCount) {
    for (; advanceCount; advanceCount--) {
        while (*currentChar != ' ' && *currentChar != '\0') {
            currentChar++;
        }

        if (*currentChar == ' ') {
            while (*currentChar == ' ') {
                currentChar++;
            }
        } else {
            currentChar++;
        }
    }

    return currentChar;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *currentChar = ti_GetDataPtr(slot);
    char *endOfFile = currentChar + ti_GetSize(slot);
    ti_Close(slot);

    unsigned int total = 0;

    while (currentChar < endOfFile) {
        currentChar = advance(currentChar, 2);
        uint8_t *winningNumbers = NULL;
        uint8_t winningNumberCount = 0;
        unsigned int score = 0;

        while (*currentChar != '|') {
            winningNumberCount++;
            winningNumbers = realloc(winningNumbers, winningNumberCount);
            winningNumbers[winningNumberCount - 1] = getNumber(currentChar);
            currentChar = advance(currentChar, 1);
        }

        currentChar = advance(currentChar, 1);

        while (*(currentChar - 1) != '\0' && currentChar < endOfFile) {
            uint8_t scratchNumber = getNumber(currentChar);

            for (uint8_t i = 0; i < winningNumberCount; i++) {
                if (scratchNumber == winningNumbers[i]) {
                    if (score) {
                        score *= 2;
                    } else {
                        score = 1;
                    }
                }
            }

            if (currentChar + 1 < endOfFile) {
                currentChar = advance(currentChar, 1);
            }
        }

        total += score;
        free(winningNumbers);
    }


    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
