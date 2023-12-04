#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

#include "asm/tools.h"

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

int main(void) {
    unsigned int slot = ti_Open("Input", "r");
    char *currentChar = ti_GetDataPtr(slot);
    char *endOfFile = currentChar + ti_GetSize(slot);
    ti_Close(slot);

    unsigned int total = 0;
    unsigned int *cardScores = NULL;
    unsigned int cardCount = 0;

    while (currentChar < endOfFile) {
        currentChar = advance(currentChar, 2);
        unsigned int *winningNumbers = NULL;
        unsigned int winningNumberCount = 0;
        unsigned int score = 0;

        while (*currentChar != '|') {
            winningNumberCount++;
            winningNumbers = realloc(winningNumbers, sizeof(unsigned int) * winningNumberCount);
            winningNumbers[winningNumberCount - 1] = getNumber(currentChar);
            currentChar = advance(currentChar, 1);
        }

        currentChar = advance(currentChar, 1);
        cardCount++;
        cardScores = realloc(cardScores, sizeof(unsigned int) * cardCount);
        cardScores[cardCount - 1] = 0;

        while (*(currentChar - 1) != '\0' && currentChar < endOfFile) {
            unsigned int scratchNumber = getNumber(currentChar);

            for (unsigned int i = 0; i < winningNumberCount; i++) {
                if (scratchNumber == winningNumbers[i]) {
                    score++;
                }
            }

            if (currentChar + 1 < endOfFile) {
                currentChar = advance(currentChar, 1);
            }
        }

        cardScores[cardCount - 1] = score;
        free(winningNumbers);
    }

    unsigned int *newCounts = malloc(sizeof(unsigned int) * cardCount);

    for (uint8_t i = 0; i < cardCount; i++) {
        newCounts[i] = 1;
    }

    for (uint8_t i = 0; i < cardCount; i++) {
        for (uint8_t j = i + 1; j < i + 1 + cardScores[i]; j++) {
            newCounts[j] += newCounts[i];
        }

        total += newCounts[i];
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintInt(total, 0);
    while (!kb_AnyKey());
    gfx_End();
}
