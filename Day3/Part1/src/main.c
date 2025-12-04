#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *bank = ti_GetDataPtr(slot);
    char *endOfFile = bank + ti_GetSize(slot);
    ti_Close(slot);
    uint8_t banklen = strlen(bank);
    unsigned int joltage = 0;

    for (; bank < endOfFile; bank += banklen + 1) {
        uint8_t b1 = 0;
        uint8_t b2 = 0;
        uint8_t b2start = 0;

        for (uint8_t i = 0; i < banklen - 1; i++) {
            if (bank[i] - '0' > b1) {
                b1 = bank[i] - '0';
                b2start = i + 1;
            }
        }

        for (uint8_t i = b2start; i < banklen; i++) {
            if (bank[i] - '0' > b2) {
                b2 = bank[i] - '0';
            }
        }

        joltage += 10 * b1 + b2;
    }

    gfx_Begin();
    gfx_PrintUInt(joltage, 1);
    while (!kb_AnyKey());
    gfx_End();
}
