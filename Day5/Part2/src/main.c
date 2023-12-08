#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include <debug.h>

struct seed_t {
    unsigned long long start;
    unsigned long long range;
};

// back to space
char *advance(char *currentChar, unsigned int backCount) {
    for (; backCount; backCount--) {
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

struct seed_t *getMappedValue(char *currentChar, char *endOfFile, struct seed_t *seeds, unsigned int *numberOfSeeds) {
    for (unsigned int i = 0; i < *numberOfSeeds; i++) {
        char *map = currentChar;

        while (*map != '\0' && map < endOfFile) {
            unsigned long long dest = strtoull(map, NULL, 10);
            map = advance(map, 1);
            unsigned long long src = strtoull(map, NULL, 10);
            map = advance(map, 1);
            unsigned long long range = strtoull(map, NULL, 10);
            map = advance(map, 1); // Skip to next row

            if (seeds[i].start < src + range && seeds[i].start + seeds[i].range > src) { // Splitting (essentially a clipping routine)
                if (seeds[i].start < src) {
                    *numberOfSeeds += 1;
                    seeds = realloc(seeds, sizeof(struct seed_t) * *numberOfSeeds);
                    seeds[*numberOfSeeds - 1].start = seeds[i].start;
                    seeds[*numberOfSeeds - 1].range = src - seeds[i].start;
                    seeds[i].range = seeds[i].start + seeds[i].range - src;
                    seeds[i].start = src;
                }

                if (seeds[i].start + seeds[i].range > src + range) {
                    *numberOfSeeds += 1;
                    seeds = realloc(seeds, sizeof(struct seed_t) * *numberOfSeeds);
                    seeds[*numberOfSeeds - 1].start = src + range;
                    seeds[*numberOfSeeds - 1].range = seeds[i].start + seeds[i].range - seeds[*numberOfSeeds - 1].start;
                    seeds[i].range -= seeds[*numberOfSeeds - 1].range;
                }

                seeds[i].start = dest + (seeds[i].start - src);
                break; // Exit loop for this item when we find a valid mapping range so we don't convert too many times
            }
        }
    }

    return seeds;
}

int main(void) {
    unsigned int slot = ti_Open("Input", "r");
    char *currentChar = ti_GetDataPtr(slot);
    char *endOfFile = currentChar + ti_GetSize(slot);
    ti_Close(slot);

    currentChar = advance(currentChar, 1); // Skip "seeds: "

    struct seed_t *seeds = NULL;
    unsigned int numberOfSeeds = 0;

    while (*currentChar != '\0') {
        numberOfSeeds++;
        seeds = realloc(seeds, sizeof(struct seed_t) * numberOfSeeds);
        seeds[numberOfSeeds - 1].start = strtoull(currentChar, NULL, 10);
        currentChar = advance(currentChar, 1);
        seeds[numberOfSeeds - 1].range = strtoull(currentChar, NULL, 10);
        currentChar = advance(currentChar, 1);
    }

    currentChar++;

    for (uint8_t map = 0; map < 7; map++) {
        currentChar = advance(currentChar, 2);

        seeds = getMappedValue(currentChar, endOfFile, seeds, &numberOfSeeds);

        while (!(*currentChar == '\0' && *(currentChar - 1) == '\0') && currentChar < endOfFile) {
            currentChar++;
        }

        currentChar++;
    }

    unsigned long long lowestLocation = seeds[0].start;

    for (unsigned int i = 0; i < numberOfSeeds; i++) {
        if (seeds[i].start < lowestLocation) {
            lowestLocation = seeds[i].start;
        }
    }

    dbg_printf("Finished: %llu\n", lowestLocation);
    //gfx_Begin();
    //gfx_SetTextXY(0, 0);
    //gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    //gfx_End();

    return 0;
}
