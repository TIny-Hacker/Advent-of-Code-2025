#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include <debug.h>

void doRow(uint48_t *row1, uint48_t *row2, uint8_t width) {
    for (uint8_t i = 0; i < width; i++) {
        if (row2[i] == 0xFFFFFF) {
            row2[i - 1] += row1[i];
            row2[i + 1] += row1[i];
        } else {
            row2[i] += row1[i];
        }
    }
}

void readLine(uint48_t *row, char *data, uint8_t width) {
    for (uint8_t i = 0; i < width; i++) {
        switch (data[i]) {
            case '.':
                row[i] = 0;
                break;
            case '^':
                row[i] = 0xFFFFFF;
                break;
            default:
                row[i] = 1;
                break;
        }
    }
}

uint48_t count(uint48_t *row, uint8_t width) {
    uint48_t total = 0;

    for (uint8_t i = 0; i < width; i++) {
        if (row[i] != 0xFFFFFF) {
            total += row[i];
        }
    }

    return total;
}

void copy(uint48_t *row1, uint48_t *row2, uint8_t width) {
    for (uint8_t i = 0; i < width; i++) {
        row1[i] = row2[i] == 0xFFFFFF ? 0 : row2[i];
    }
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *data = ti_GetDataPtr(slot);
    char *endOfFile = data + ti_GetSize(slot);
    ti_Close(slot);

    uint8_t width = strlen(data);
    uint48_t *row1 = malloc(width * sizeof(uint48_t));
    uint48_t *row2 = malloc(width * sizeof(uint48_t));
    readLine(row1, data, width);
    data += width + 1;

    while (data < endOfFile) {
        readLine(row2, data, width);
        doRow(row1, row2, width);
        copy(row1, row2, width);
        data += width + 1;
    }

    dbg_printf("%llu\n", (unsigned long long)count(row1, width));
    return 0;
}
