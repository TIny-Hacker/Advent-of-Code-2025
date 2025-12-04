#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

bool canMove(int x, int y, uint8_t dim) {
    uint8_t surround = 0;

    for (int cy = -1; cy < 2; cy++) {
        for (int cx = -1; cx < 2; cx++) {
            if (x + cx >= 0 && x + cx < dim && y + cy >= 0 && y + cy < dim && !(!cx && !cy)) {
                surround += gfx_GetPixel(x + cx, y + cy) != 255;
            }
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
    gfx_Begin();

    for (uint8_t y = 0; y < dim; y++, row += dim + 1) {
        for (uint8_t x = 0; x < dim; x++) {
            if (row[x] == '@') {
                gfx_SetPixel(x, y);
            }
        }
    }

    unsigned int changed;

    do {
        changed = 0;
        gfx_SetColor(5);

        for (int y = 0; y < dim; y++) {
            for (int x = 0; x < dim; x++) {
                if (!gfx_GetPixel(x, y) && canMove(x, y, dim)) {
                    gfx_SetPixel(x, y);
                    changed++;
                }
            }
        }

        gfx_SetColor(255);

        for (uint8_t y = 0; y < dim; y++) {
            for (uint8_t x = 0; x < dim; x++) {
                if (gfx_GetPixel(x, y) == 5) {
                    gfx_SetPixel(x, y);
                }
            }
        }

        total += changed;
    } while (changed);

    gfx_SetTextXY(0, dim + 1);
    gfx_PrintUInt(total, 1);
    while (!kb_AnyKey());
    gfx_End();
}
