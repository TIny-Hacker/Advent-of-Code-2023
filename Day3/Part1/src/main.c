#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include "asm/tools.h"

bool isValidNumber(char *startOfFile, char *endOfFile, char *currentChar, uint8_t lineLength) {
    if (*currentChar < '0' || *currentChar > '9') {
        return false;
    }

    for (int8_t x = -1; x < 2; x++) {
        for (int8_t y = -1; y < 2; y++) {
            char *testChar = currentChar + x + y * lineLength;

            if (testChar >= startOfFile && testChar < endOfFile && *testChar != '\0') { // Ensure valid coordinate in map
                if ((*testChar < '0' || *testChar > '9') && *testChar != '.') {
                    return true;
                }
            }
        }
    }

    return false;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *startOfFile = ti_GetDataPtr(slot);
    char *endOfFile = startOfFile + ti_GetSize(slot);

    ti_Close(slot);

    unsigned int total = 0;
    uint8_t lineLength = strlen(startOfFile) + 1;
    unsigned int currentPartID = 0;

    for (char *currentChar = startOfFile; currentChar < endOfFile; currentChar++) {
        if (currentPartID == 0 && *currentChar >= '0' && *currentChar <= '9') {
            currentPartID = getNumber(currentChar);
        } else if (*currentChar < '0' || *currentChar > '9') {
            currentPartID = 0;
        }


        if (isValidNumber(startOfFile, endOfFile, currentChar, lineLength)) {
            total += currentPartID;

            while (*currentChar >= '0' && *currentChar <= '9') { // Skip to the next thing to only count IDs once
                currentChar++;
            }

            currentPartID = 0;
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
