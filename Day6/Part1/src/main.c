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
    ti_Close(slot);

    unsigned int total = 1;

    unsigned int *raceTimes = NULL;
    uint8_t raceCount = 0;

    currentChar = advance(currentChar, 1);

    while (*(currentChar - 1) != '\0') { // Get times
        raceCount++;
        raceTimes = realloc(raceTimes, sizeof(unsigned int) * raceCount);
        raceTimes[raceCount - 1] = getNumber(currentChar);
        currentChar = advance(currentChar, 1);
    }

    currentChar = advance(currentChar, 1);

    for (uint8_t i = 0; i < raceCount; i++) {
        unsigned int distance = getNumber(currentChar);
        currentChar = advance(currentChar, 1);

        unsigned int recordsBroken = 0;

        for (unsigned int j = 0; j < raceTimes[i]; j++) {
            if (j * (raceTimes[i] - j) > distance) {
                recordsBroken++;
            }
        }

        total *= recordsBroken;
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
