#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

uint8_t doLine(uint8_t y, uint8_t width) {
    uint8_t total = 0;
    gfx_SetColor('S');

    for (uint8_t x = 0; x < width; x++) {
        if (gfx_GetPixel(x, y) == 'S') {
            if (gfx_GetPixel(x, y + 1) == '^') {
                gfx_SetPixel(x - 1, y + 1);
                gfx_SetPixel(x + 1, y + 1);
                total++;
            } else {
                gfx_SetPixel(x, y + 1);
            }
        }
    }

    return total;
}

int main(void) {
    uint8_t slot = ti_Open("Input", "r");
    char *data = ti_GetDataPtr(slot);
    char *endOfFile = data + ti_GetSize(slot);
    ti_Close(slot);
    gfx_Begin();
    unsigned int x = 0;
    uint8_t y = 0;
    uint8_t width = strlen(data);

    while (data < endOfFile) {
        if (*data == '\0') {
            y++;
            x = 0;
        } else {
            gfx_SetColor(*data);
            gfx_SetPixel(x, y);
            x++;
        }

        data++;
    }

    unsigned int total = 0;

    for (uint8_t y2 = 0; y2 < y; y2++) {
        total += doLine(y2, width);
    }

    gfx_SetTextXY(0, y + 2);
    gfx_PrintUInt(total, 1);
    while (!kb_AnyKey());
    gfx_End();
    return 0;
}
