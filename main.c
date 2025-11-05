#include "tigr.h"
#include "tigr.c"

#include <stdbool.h>

#define BMP_WIDTH 240
#define BMP_HEIGHT 120
#define BMP_MAX_PIX (BMP_WIDTH * BMP_HEIGHT)

TPixel defPix = {0xff, 0xff, 0xff, 255};
TPixel erasePix = {0x9c, 0x97, 0xbd, 255};

bool isErasing = false;

typedef struct {
    int x;
    int y;
} t_pix;

int update_text_buffer(Tigr*, int, char*);
void fast_update_bmp(Tigr*, int, int);

int main(int argc, char *argv[])
{
    char text_buffer[4096] = {0};
    int curr_char = 0;
    t_pix pix;
    t_pix start_pix;
    t_pix end_pix;
    int buttons;
    bool ready_draw = false;
    Tigr *screen = tigrWindow(240, 120, "FSM VISUALIZER", 0);
    float elapsed_time = tigrTime();
    tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));

    while (!tigrClosed(screen))
    {
        //tigrPrint(screen, tfont, 0, 0, tigrRGB(0xff, 0xff, 0xff), text_buffer);
        tigrMouse(screen, &pix.x, &pix.y, &buttons);
        if(buttons == 1) {
            fast_update_bmp(screen, pix.x, pix.y);
        }

        curr_char = update_text_buffer(screen, curr_char, text_buffer);
        tigrUpdate(screen);
    }
    tigrFree(screen);
    return 0;
}
int update_text_buffer(Tigr *screen, int curr_char, char *buffer)
{
    char c = (char)tigrReadChar(screen);
    if(c == '<')
        tigrSaveImage("draw.png", screen);
    if(c == 'c')
        tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));
    if(c == 'b')
        isErasing = !isErasing;
    if (c != NULL)
        buffer[++curr_char] = c;
    if (curr_char % 90 == 0)
        buffer[curr_char] = '\n';
    
    return curr_char;
}

void fast_update_bmp(Tigr* bmp, int x, int y) {
    if(y * BMP_WIDTH + x > BMP_MAX_PIX) return;
    if(!isErasing)
        bmp->pix[y * BMP_WIDTH + x] = defPix;
    else
        bmp->pix[y * BMP_WIDTH + x] = erasePix;
}