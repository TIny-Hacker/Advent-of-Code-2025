#include <fileioc.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <debug.h>

uint8_t digits(uint48_t n) {
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}

uint48_t getf(uint8_t d) {
    uint48_t f = 1;

    for (; d; d--) {
        f *= 10;
    }

    return f;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *entry = ti_GetDataPtr(slot) - 1;
    char *endOfFile = entry + ti_GetSize(slot);
    ti_Close(slot);

    uint48_t total = 0;

    while (entry < endOfFile) {
        uint48_t n1 = (uint48_t)strtoll(++entry, &entry, 10);
        uint48_t n2 = (uint48_t)strtoll(++entry, &entry, 10);

        for (; n1 <= n2; n1++) {
            uint8_t d = digits(n1);

            if (d % 2) {
                n1 = getf(d);
                continue;
            }

            uint48_t f = getf(d / 2);

            if (n1 / f == n1 % f) {
                total += n1;
            }
        }
    }

    dbg_printf("%llu\n", (unsigned long long)total);

    return 0;
}
