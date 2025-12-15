#include <fileioc.h>
#include <string.h>
#include <stdlib.h>

#include <debug.h>

unsigned int readNum(char *s, unsigned int skip, uint8_t maxDigits) {
    unsigned int n = 0;

    for (uint8_t i = 0; i < maxDigits; i++, s += skip) {
        if (*s != ' ') {
            n *= 10;
            n += *s - '0';
        }
    }

    return n;
}

char *nextTok(char *s) {
    while (*s == ' ') {
        s++;
    }

    return s;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *op = ti_GetDataPtr(slot);
    char *endOfFile = op + ti_GetSize(slot);
    ti_Close(slot);

    char *col = op;
    uint8_t rows = 0;
    uint48_t total = 0;
    unsigned int skip = strlen(op) + 1;

    while (*op != '+' && *op != '*') {
        op += skip;
        rows++;
        dbg_printf("%p\n", op);
    }


    while (op < endOfFile && *op != '\0') {
        uint48_t ans = 0 + (*op == '*');
        unsigned int n = readNum(col, skip, rows);

        do {
            dbg_printf("%d, ", n);
            if (*op == '*') {
                ans *= n;
            } else {
                ans += n;
            }

            n = readNum(++col, skip, rows);
        } while (*col != '\0' && n);

        dbg_printf("%c\n", *op);

        total += ans;
        op = nextTok(op + 1);
        col++;
    }

    dbg_printf("%llu", (unsigned long long)total);
}
