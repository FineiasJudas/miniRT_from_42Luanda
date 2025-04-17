#include "minirt.h"

void draw_circle(t_color image[HEIGHT][WIDTH], int cx, int cy, int radius, t_color color)
{
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx*dx + dy*dy <= radius*radius)
                image[y][x] = color;
        }
    }
}
    