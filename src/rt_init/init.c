#include "minirt.h"


t_scene *create_scene(void)
{
    t_scene *scene = malloc(sizeof(t_scene));
    if (!scene)
        printf("Erro ao alocar memória para a cena");

    // Inicializa listas como NULL
    scene->lights = NULL;
    scene->spheres = NULL;
   // scene->planes = NULL;
   // scene->cylinders = NULL;

    // Inicializa campos escalares com valores padrão (opcional)
    scene->ambient.ratio = 0;
    scene->ambient.color = (t_color){0, 0, 0};
    scene->camera.origin = (t_vector){0, 0, 0};
    scene->camera.direction = (t_vector){0, 0, 1};
    scene->camera.fov = 70;

    return scene;
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

void put_pixel(t_image *img, int x, int y, int color)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    char *dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

