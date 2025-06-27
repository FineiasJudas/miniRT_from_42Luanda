#include "minirt.h"

void    create_data(t_data data)
{
    (void)data;
    data.ambient = NULL;
    data.cam_origin = NULL;
    data.camera = NULL;
    data.cube = NULL;
    data.cylinder = NULL;
    data.light = NULL;
    data.mlx = NULL;
    data.plane = NULL;
    data.render = NULL;
    data.sphere = NULL;
    data.win = NULL;
    data.img.addr = NULL;
    data.img.img_ptr = NULL;
    data.sphere_l = NULL;
    data.cylinder_l = NULL;
    data.lights_l = NULL;
}

void init_image(t_image *img, void *mlx, int width, int height)
{
    img->width = width;
    img->height = height;
    img->img_ptr = mlx_new_image(mlx, width, height);
    img->addr = mlx_get_data_addr(img->img_ptr,
            &img->bits_per_pixel,
            &img->line_length,
            &img->endian);
}

void put_pixel(t_image *img, int x, int y, t_color color)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    char *dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int *)dst = color_to_int(color);
}

