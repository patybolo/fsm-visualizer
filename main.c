#include "tigr.h"
#include "tigr.c"

#include <stdbool.h>

#define BMP_WIDTH 240
#define BMP_HEIGHT 120
#define BMP_MAX_PIX (BMP_WIDTH * BMP_HEIGHT)

TPixel defPix = {0xff, 0xff, 0xff, 255};
TPixel erasePix = {0x9c, 0x97, 0xbd, 255};

bool isErasing = false;
int buttons;

typedef struct
{
    int x;
    int y;
} t_pix;

int update_text_buffer(Tigr *, int, char *);
void handle_mouse(Tigr *);
void fast_update_bmp(Tigr *, int, int);

int main(int argc, char *argv[])
{
    char text_buffer[4096] = {0};
    int curr_char = 0;
    bool ready_draw = false;
    Tigr *screen = tigrWindow(360, 320, "paint thingy", 4);
    float elapsed_time = tigrTime();
    tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));

    while (!tigrClosed(screen))
    {
        // tigrPrint(screen, tfont, 0, 0, tigrRGB(0xff, 0xff, 0xff), text_buffer);
        handle_mouse(screen);
        curr_char = update_text_buffer(screen, curr_char, text_buffer);
        tigrUpdate(screen);
    }
    tigrFree(screen);
    return 0;
}

int update_text_buffer(Tigr *screen, int curr_char, char *buffer)
{
    char c = (char)tigrReadChar(screen);
    if (c == '<')
        tigrSaveImage("draw.png", screen);
    if (c == 'c')
        tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));
    if (c == 'b')
        isErasing = !isErasing;
    if (c != NULL)
        buffer[++curr_char] = c;
    if (curr_char % 90 == 0)
        buffer[curr_char] = '\n';

    return curr_char;
}

void handle_mouse(Tigr *screen)
{
    t_pix pix;
    tigrMouse(screen, &pix.x, &pix.y, &buttons);
    if (buttons == 1)
    {
        if (!isErasing)
            tigrPlot(screen, pix.x, pix.y, defPix);
        else
            tigrPlot(screen, pix.x, pix.y, erasePix);
    }
}

void fast_update_bmp(Tigr *bmp, int x, int y)
{
    if (y * BMP_WIDTH + x > BMP_MAX_PIX)
        return;
    if (!isErasing)
        bmp->pix[y * BMP_WIDTH + x] = defPix;
    else
        bmp->pix[y * BMP_WIDTH + x] = erasePix;
}