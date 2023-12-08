#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include <debug.h>

// I just changed data types and other stuff to speed it up for this and then manually modified the input. If it works, it works :/

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

    unsigned long long fails = 0;

    currentChar = advance(currentChar, 1);
    unsigned long long time = strtoull(currentChar, NULL, 10);
    currentChar = advance(currentChar, 2);
    unsigned long long distance = strtoull(currentChar, NULL, 10);

    for (unsigned long long j = 0; j < time; j++) {
        if (j * (time - j) > distance) {
            break;
        } else {
            fails++;
        }
    }

    //gfx_Begin();
    //gfx_SetTextXY(0, 0);
    //gfx_PrintInt(total, 0);
    dbg_printf("%llu", (time + 1) - fails * 2);
    while (!kb_AnyKey());
    //gfx_End();
}
