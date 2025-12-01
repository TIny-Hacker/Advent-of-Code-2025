#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *entry = ti_GetDataPtr(slot);
    char *endOfFile = entry + ti_GetSize(slot);
    ti_Close(slot);
    int currentNum = 50;
    int score = 0;

    while (entry < endOfFile) {
        if (*entry == 'L') {
            currentNum -= atoi(++entry) % 100;

            if (currentNum < 0) {
                currentNum += 100;
            }
        } else {
            currentNum += atoi(++entry);
            currentNum %= 100;
        }

        if (!currentNum) {
            score++;
        }

        while (*(entry - 1) != '\0' && entry < endOfFile) {
            entry++;
        }
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintUInt(score, 1);
    while(!kb_AnyKey());
    gfx_End();
    return 0;
}
