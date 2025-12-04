#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

bool canMove(char *data, uint8_t x, uint8_t y, uint8_t dim) {
    uint8_t surround = 0;

    if (y) {
        if (x) {
            surround += *(data - (dim + 1) - 1) == '@'; // (-1, -1)
        }

        surround += *(data - (dim + 1)) == '@'; // (0, -1)

        if (x < dim - 1) {
            surround += *(data - (dim + 1) + 1) == '@'; // (1, -1)
        }
    }

    if (x) {
        surround += *(data - 1) == '@'; // (-1, 0)
    }

    if (x < dim - 1) {
        surround += *(data + 1) == '@'; // (1, 0)
    }

    if (y < dim - 1) {
        if (x) {
            surround += *(data + (dim + 1) - 1) == '@'; // (-1, 1)
        }

        surround += *(data + (dim + 1)) == '@'; // (0, 1)

        if (x < dim - 1) {
            surround += *(data + (dim + 1) + 1) == '@'; // (1, 1)
        }
    }

    return surround < 4 ? true : false;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *row = ti_GetDataPtr(slot);
    ti_Close(slot);
    uint8_t dim = strlen(row);
    unsigned int total = 0;

    for (uint8_t y = 0; y < dim; y++, row += dim + 1) {
        for (uint8_t x = 0; x < dim; x++) {
            if (row[x] == '@') {
                total += canMove(&(row[x]), x, y, dim);
            }
        }
    }

    gfx_Begin();
    gfx_PrintUInt(total, 1);
    while (!kb_AnyKey());
    gfx_End();
}
