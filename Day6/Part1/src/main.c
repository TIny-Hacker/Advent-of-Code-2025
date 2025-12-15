#include <fileioc.h>
#include <string.h>
#include <stdlib.h>

#include <debug.h>

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


    while (op < endOfFile) {
        char *row = col;
        uint48_t ans = 0 + (*op == '*');

        for (uint8_t i = 0; i < rows; i++) {
            dbg_printf("%llu, ", strtoll(nextTok(row), NULL, 10));
            if (*op == '*') {
                ans *= strtoll(nextTok(row), NULL, 10);
            } else {
                ans += strtoll(nextTok(row), NULL, 10);
            }

            row += skip;
        }

        dbg_printf("%c\n", *op);

        total += ans;
        op = nextTok(op + 1);
        col = op - rows * skip;
    }

    dbg_printf("%llu", (unsigned long long)total);
}
