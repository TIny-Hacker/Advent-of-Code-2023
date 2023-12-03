#include <fileioc.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include "asm/tools.h"

#include <debug.h>

char *findNumberStart(char *currentChar) {
    while (*currentChar >= '0' && *currentChar <= '9') {
        currentChar--;
    }

    return currentChar + 1;
}

unsigned int getGearRatio(char *startOfFile, char *endOfFile, char *currentChar, uint8_t lineLength) {
    if (*currentChar != '*') {
        return 0;
    }

    unsigned int id1 = 0;
    unsigned int id2 = 0;

    for (int8_t x = -1; x < 2; x++) {
        for (int8_t y = -1; y < 2; y++) {
            char *testChar = currentChar + x + y * lineLength;

            if (testChar >= startOfFile && testChar < endOfFile && *testChar != '\0') { // Ensure valid coordinate in map
                if (*testChar >= '0' && *testChar <= '9') {
                    if (x == -1 || (*(testChar - 1) < '0' || *(testChar - 1) > '9')) {
                        if (!id1) {
                            id1 = getNumber(findNumberStart(testChar));
                        } else if (!id2) {
                            id2 = getNumber(findNumberStart(testChar));
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return id1 * id2;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *startOfFile = ti_GetDataPtr(slot);
    char *endOfFile = startOfFile + ti_GetSize(slot);

    ti_Close(slot);

    unsigned long total = 0;
    uint8_t lineLength = strlen(startOfFile) + 1;

    for (char *currentChar = startOfFile; currentChar < endOfFile; currentChar++) {
        total += getGearRatio(startOfFile, endOfFile, currentChar, lineLength);
    }

    dbg_printf("%lu", total);
    while (!kb_AnyKey());
}
