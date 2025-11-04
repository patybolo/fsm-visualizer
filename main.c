#include "tigr.h"
#include "tigr.c"

#include <stdbool.h>

TPixel defPix = {0xff, 0xff, 0xff, 255};

typedef struct {
    int x;
    int y;
} t_pix;

int update_text_buffer(Tigr*, int, char*);

int main(int argc, char *argv[])
{
    char text_buffer[4096] = {0};
    int curr_char = 0;
    t_pix pix;
    t_pix start_pix;
    t_pix end_pix;
    int buttons;
    bool ready_draw = false;
    Tigr *screen = tigrWindow(240, 120, "FSM VISUALIZER", 3);
    float elapsed_time = tigrTime();
    tigrClear(screen, tigrRGB(0x9c, 0x97, 0xbd));

    while (!tigrClosed(screen))
    {
    
        //tigrPrint(screen, tfont, 0, 0, tigrRGB(0xff, 0xff, 0xff), text_buffer);

        tigrMouse(screen, &pix.x, &pix.y, &buttons);
        if(buttons == 1) {
            tigrPlot(screen, pix.x, pix.y, defPix);
        }

        if(buttons == 4 && !ready_draw) {
            start_pix = pix;
            elapsed_time = tigrTime();
            ready_draw = true;
        }
        else if(buttons == 4 && ready_draw && tigrTime() >= elapsed_time + 0.01f) {
            end_pix = pix;
            tigrLine(screen, start_pix.x,start_pix.y, end_pix.x, end_pix.y, defPix);
            ready_draw = false;
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
    if (c != NULL)
        buffer[++curr_char] = c;
    if (curr_char % 90 == 0)
        buffer[curr_char] = '\n';
    
    return curr_char;
}