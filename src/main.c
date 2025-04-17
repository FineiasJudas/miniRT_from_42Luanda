/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/02/11 10:22:02 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


t_vector vec_sub(t_vector a, t_vector b) {
    return (t_vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

float vec_dot(t_vector a, t_vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_vector vec_normalize(t_vector v) {
    float len = sqrtf(vec_dot(v, v));
    return (t_vector){v.x / len, v.y / len, v.z / len};
}

int intersect_ray_sphere(t_ray ray, t_sphere s, float *t) {
    t_vector oc = vec_sub(ray.origin, s.center);
    float a = vec_dot(ray.direction, ray.direction);
    float b = 2.0 * vec_dot(oc, ray.direction);
    float c = vec_dot(oc, oc) - s.radius * s.radius;
    float disc = b*b - 4*a*c;
        
    if (disc < 0)
        return 0;
    *t = (-b - sqrtf(disc)) / (2.0 * a);
    return (*t > 0);
}


int main(int ac, char *av[])
{
    (void)ac;
    (void)av;
    t_scene *scene;
    scene = create_scene();

    printf("Hello MiniRT %li\n", ft_strlen("1973"));
    printf("A potencia de 5 evlevado a 3 é %f\n", pow(5, 3));

    int rs = parse_rt_file("models/test.rt", scene);
    printf("RESULT: %d", rs);

    void *mlx = mlx_init();
    void *win = mlx_new_window(mlx, 800, 600, "miniRT");
    t_image img;
    init_image(&img, mlx, 800, 600);

    t_sphere sphere = {{0, 0, -5}, 1.5, {255, 255, 255}}; // centro em Z negativo
    t_vector cam_origin = {0, 0, 0}; // origem da câmera

    for (int y = 0; y < 600; y++) {
        for (int x = 0; x < 800; x++) {
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -1}); // tela em z = -1

            t_ray ray = {cam_origin, ray_dir};
            float t;

            if (intersect_ray_sphere(ray, sphere, &t))
                put_pixel(&img, x, y, sphere.color);
            else
                put_pixel(&img, x, y, (t_color){0, 0, 0}); // fundo preto
        }
    }

    mlx_put_image_to_window(mlx, win, img.img_ptr, 0, 0);
    mlx_loop(mlx);
    return 0;

    /*
    void *mlx;
    void *win;
    t_image img;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 800, 600, "miniRT");

    init_image(&img, mlx, 800, 600);

    int rgb[10] = {
        0xff0000,
        0xff0000,
        0xff0000,
        0xff0000,
        0xff0000,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00};
    int i = -1;
    int yc = 300;
    int xc = 400;
    int r = 200;
    while (++i < 7)
    {
        for (int y = 0; y < 600; y++)
        {

            for (int x = 0; x < 800; x++)
            {
                int dx = x-xc;
                int dy = y-yc;
                if (dx*dx + dy*dy <= r*r)
                    put_pixel(&img, x, y, rgb[(x % 11)]);

            }
        }
        mlx_put_image_to_window(mlx, win, img.img_ptr, 0, 0);
        sleep(2);
    }

    mlx_loop(mlx);
    return (0);
    */
}
