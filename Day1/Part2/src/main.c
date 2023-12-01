#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

#include <debug.h>

// This is probably not very good
uint8_t isTextNumber(char *start) {
    if (!strncmp(start, "one", 3)) {
        return 1;
    } else if (!strncmp(start, "two", 3)) {
        return 2;
    } else if (!strncmp(start, "three", 5)) {
        return 3;
    } else if (!strncmp(start, "four", 4)) {
        return 4;
    } else if (!strncmp(start, "five", 4)) {
        return 5;
    } else if (!strncmp(start, "six", 3)) {
        return 6;
    } else if (!strncmp(start, "seven", 5)) {
        return 7;
    } else if (!strncmp(start, "eight", 5)) {
        return 8;
    } else if (!strncmp(start, "nine", 4)) {
        return 9;
    }

    return 0;
}

int main(void) {
    unsigned int total = 0;

    uint8_t slot = ti_Open("Input", "r");
    char *currentLine = ti_GetDataPtr(slot);
    char *endOfFile = currentLine + ti_GetSize(slot);
    ti_Close(slot);

    uint8_t digit1;
    uint8_t digit2;
    uint8_t lineLength;

    while (currentLine < endOfFile) {
        digit1 = 0;
        digit2 = 0;
        lineLength = strlen(currentLine);

        if (currentLine + lineLength > endOfFile) {
            lineLength = endOfFile - currentLine;
        }

        for (char *currentChar = currentLine; currentChar <= currentLine + lineLength; currentChar++) {
            if (*currentChar >= '0' && *currentChar <= '9') {
                digit1 = *currentChar - '0';
            } else {
                digit1 += isTextNumber(currentChar);
            }

            if (digit1) {
                break;
            }
        }

        for (char *currentChar = currentLine + lineLength; currentChar >= currentLine; currentChar--) {
            if (*currentChar >= '0' && *currentChar <= '9') {
                digit2 = *currentChar - '0';
                break;
            } else {
                digit2 += isTextNumber(currentChar);
            }

            if (digit2) {
                break;
            }
        }

        total += digit1 * 10 + digit2;
        currentLine = currentLine + lineLength + 1;
    }


    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
