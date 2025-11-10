#include "tigr.h"
#include "tigr.c"

#include <stdbool.h>

#define BMP_WIDTH 320
#define BMP_HEIGHT 240
#define BMP_MAX_PIX (BMP_WIDTH * BMP_HEIGHT)

TPixel defPix = {0xff, 0xff, 0xff, 255};
TPixel erasePix = {0x9c, 0x97, 0xbd, 255};

bool isErasing = false;
int eraserSize = 5;
int buttons;

typedef struct
{
    int x;
    int y;
} t_pix;

int update_text_buffer(Tigr*, int, char *);
void handle_mouse(Tigr*, bool);
void handle_cmd(Tigr*);
void fast_update_bmp(Tigr*, int, int);
void erase(Tigr*, int, int);

int main(int argc, char *argv[])
{
    char text_buffer[4096] = {0};
    int curr_char = 0;
    bool ready_draw = false;
    Tigr *screen = tigrWindow(BMP_WIDTH, BMP_HEIGHT, "paint thingy", 4);
    float elapsed_time = tigrTime();
    tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));
    while (!tigrClosed(screen))
    {
        handle_mouse(screen, true);
        handle_cmd(screen);
        tigrUpdate(screen);

        // tigrPrint(screen, tfont, 0, 0, tigrRGB(0xff, 0xff, 0xff), text_buffer);
        // curr_char = update_text_buffer(screen, curr_char, text_buffer);
    }
    tigrFree(screen);
    return 0;
}

void handle_cmd(Tigr *screen)
{
    char c = (char)tigrReadChar(screen);
    if (c == '<')
        tigrSaveImage("draw.png", screen);
    if (c == 'c')
        tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));
    if (c == 'b')
        isErasing = !isErasing;
    if (c == 'i' && eraserSize < 25)
        eraserSize+=2;
    if (c == 'd' && eraserSize > 2)
        eraserSize-=2;
}

int update_text_buffer(Tigr *screen, int curr_char, char *buffer)
{
    char c = (char)tigrReadChar(screen);
    if (c != NULL)
        buffer[++curr_char] = c;
    return curr_char;
}

void handle_mouse(Tigr *screen, bool isFastUpdate)
{
    t_pix pix;
    tigrMouse(screen, &pix.x, &pix.y, &buttons);
    if (buttons)
    {
        if (!isErasing && !isFastUpdate)
        {
            tigrPlot(screen, pix.x, pix.y, defPix);
        }
        else if (!isErasing && isFastUpdate)
        {
            fast_update_bmp(screen, pix.x, pix.y);
        }

        else
        {
            tigrPlot(screen, pix.x, pix.y, erasePix);
        }
    }
}

void fast_update_bmp(Tigr *bmp, int x, int y)
{
    if (y * BMP_WIDTH + x > BMP_MAX_PIX || x < 0 || y < 0)
        return;
    bmp->pix[y * BMP_WIDTH + x] = defPix;
}

void erase(Tigr* screen, int center_x, int center_y) {
    /*

        xxxxx
        xxxxx
        xxxxx -> eraserSize = 5
        xxxxx
        xxxxx
        center = eraserSize - 1 / 2

    */

    tigrPlot(screen, center_x, center_y, erasePix);
    tigrPlot(screen, center_x, center_y, erasePix);
    tigrPlot(screen, center_x, center_y, erasePix);
    tigrPlot(screen, center_x, center_y, erasePix);
    tigrPlot(screen, center_x, center_y, erasePix);
}