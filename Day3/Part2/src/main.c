#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include <debug.h>

#define MAX_BATTERIES   12

uint48_t getf(uint8_t d) {
    uint48_t f = 1;

    for (; d; d--) {
        f *= 10;
    }

    return f;
}

uint48_t getJoltage(char *bank, uint8_t batleft, uint8_t max, uint8_t start) {
    uint8_t b = 0;
    uint8_t newStart = 0;

    for (uint8_t i = start; i < max - batleft; i++) {
        if (bank[i] - '0' > b) {
            b = bank[i] - '0';
            newStart = i + 1;
        }
    }

    if (batleft) {
        return b * getf(batleft) + getJoltage(bank, batleft - 1, max, newStart);
    }

    return b;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *bank = ti_GetDataPtr(slot);
    char *endOfFile = bank + ti_GetSize(slot);
    ti_Close(slot);
    uint8_t banklen = strlen(bank);
    uint48_t joltage = 0;

    for (; bank < endOfFile; bank += banklen + 1) {
        joltage += getJoltage(bank, MAX_BATTERIES - 1, banklen, 0);
    }

    dbg_printf("%llu\n", (unsigned long long)joltage);
}
