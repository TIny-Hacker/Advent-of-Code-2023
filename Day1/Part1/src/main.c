#include <fileioc.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

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
        lineLength = strlen(currentLine);

        for (char *currentChar = currentLine; currentChar <= currentLine + lineLength; currentChar++) {
            if (*currentChar >= '0' && *currentChar <= '9') {
                digit1 = *currentChar - '0';
                break;
            }
        }

        for (char *currentChar = currentLine + lineLength; currentChar >= currentLine; currentChar--) {
            if (*currentChar >= '0' && *currentChar <= '9') {
                digit2 = *currentChar - '0';
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
