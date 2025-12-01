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
    int8_t currentNum = 50;
    unsigned int score = 0;

    while (entry < endOfFile) {
        unsigned int turn = atoi(entry + 1);

        for (unsigned int i = 0; i < turn; i++) { // Slow and steady wins the race (where the race is me being lazy)
            if (*entry == 'L') {
                currentNum = !currentNum ? 99 : currentNum - 1;
            } else {
                currentNum = (currentNum == 99) ? 0 : currentNum + 1;
            }

            if (!currentNum) {
                score++;
            }
        }

        do {
            entry++;
        } while (*(entry - 1) != '\0' && entry < endOfFile);
    }

    gfx_Begin();
    gfx_SetTextXY(0, 0);
    gfx_PrintUInt(score, 1);
    while(!kb_AnyKey());
    gfx_End();
    return 0;
}
