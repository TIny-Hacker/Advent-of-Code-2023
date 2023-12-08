#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include <debug.h>

// Advance to space
char *advance(char *currentChar, unsigned int advanceCount) {
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

unsigned long long getMappedValue(char *map, char *endOfFile, unsigned long long value) {
    while (*map != '\0' && map < endOfFile) {
        unsigned long long dest = strtoull(map, NULL, 10);
        map = advance(map, 1);
        unsigned long long src = strtoull(map, NULL, 10);
        map = advance(map, 1);
        unsigned long long range = strtoull(map, NULL, 10);
        map = advance(map, 1); // Skip to next row

        if (value >= src && value <= src + range) {
            return dest + (value - src);
        }
    }

    return value; // Unmapped values are mapped to themselves
}

int main(void) {
    unsigned int slot = ti_Open("Input", "r");
    char *currentChar = ti_GetDataPtr(slot);
    char *endOfFile = currentChar + ti_GetSize(slot);
    ti_Close(slot);

    currentChar = advance(currentChar, 1); // Skip "seeds: "

    unsigned long long *seeds = NULL;
    uint8_t numberOfSeeds = 0;

    while (*currentChar != '\0') {
        numberOfSeeds++;
        seeds = realloc(seeds, sizeof(unsigned long long) * numberOfSeeds);
        seeds[numberOfSeeds - 1] = strtoull(currentChar, NULL, 10);
        currentChar = advance(currentChar, 1);
    }

    currentChar++;
    unsigned long long lowestLocation = -1;

    for (uint8_t map = 0; map < 7; map++) { // Repeat 7 times for all maps
        currentChar = advance(currentChar, 2);

        for (uint8_t i = 0; i < numberOfSeeds; i++) {
            seeds[i] = getMappedValue(currentChar, endOfFile, seeds[i]);

            if (map == 6 && seeds[i] < lowestLocation) {
                lowestLocation = seeds[i];
            }
        }

        while (!(*currentChar == '\0' && *(currentChar - 1) == '\0') && currentChar < endOfFile) {
            currentChar++;
        }

        currentChar++;
    }

    dbg_printf("%llu\n", lowestLocation);
    //gfx_Begin();
    //gfx_SetTextXY(0, 0);
    //gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    //gfx_End();
}
