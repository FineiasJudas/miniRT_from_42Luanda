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

/*FJILAIA-> CRIAR UM OU MAIS FICHEIRO PARA FUNCOES DE OPERACOES COM VECTORES */

// INICIO OPERACOES COM VECTORES
t_vector vec_sub(t_vector a, t_vector b)
{
    return (t_vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

float vec_dot(t_vector a, t_vector b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_vector vec_normalize(t_vector v)
{
    float len = sqrtf(vec_dot(v, v));
    if (len == 0)
        return (t_vector){0, 0, 0}; // evitar divisão por 0
    return (t_vector){v.x / len, v.y / len, v.z / len};
}

t_vector vec_scale(t_vector v, float s)
{
    return (t_vector){v.x * s, v.y * s, v.z * s};
}

t_vector vec_add(t_vector a, t_vector b)
{
    return (t_vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

// FIM OPERACOES COM VECTORES

t_color scale_color(t_color color, float factor)
{
    return (t_color){fmin(color.r * factor, 255),
                     fmin(color.g * factor, 255),
                     fmin(color.b * factor, 255)};
}

int intersect_ray_sphere(t_ray ray, t_sphere s, float *t)
{
    t_vector oc = vec_sub(ray.origin, s.center);
    float a = vec_dot(ray.direction, ray.direction);
    float b = 2.0 * vec_dot(oc, ray.direction);
    float c = vec_dot(oc, oc) - s.radius * s.radius;
    float disc = b * b - 4 * a * c;

    //    t_vector hit_point = vec_add(ray.origin, vec_scale(ray.direction, *t));

    if (disc < 0)
        return 0;
    *t = (-b - sqrtf(disc)) / (2.0 * a);
    return (*t > 0);
}

void render_scene(t_data *data)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
            t_ray ray = {data->cam_origin, ray_dir};
            float t;

            if (intersect_ray_sphere(ray, data->sphere, &t))
            {
                t_vector hit = vec_add(ray.origin, vec_scale(ray.direction, t));
                t_vector normal = vec_normalize(vec_sub(hit, data->sphere.center));
                t_vector light_dir = vec_normalize(vec_sub(data->light_pos, hit));
                float intensity = fmax(0.0, vec_dot(normal, light_dir));
                t_color color = scale_color(data->sphere.color, intensity);
                put_pixel(&data->img, x, y, color);
            }
            else
            {
                put_pixel(&data->img, x, y, (t_color){0, 0, 0});
            }
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

// Função de callback para o evento de pressionamento de tecla, para testar a mudanca na posicao da luz)
int key_press(int keycode, t_data *data)
{
    printf("Nova luz.z = %d\n", keycode);

    if (keycode == 65362)
    {
        // cima z
        data->light_pos.z = fmod(data->light_pos.z + 1, 100);
        printf("Nova luz.z = %f\n", data->light_pos.z);
    }
    else if (keycode == 65361)
    {
        // esquerda x
        data->light_pos.x = fmod(data->light_pos.x + 1, 100);
        printf("Nova luz.x = %f\n", data->light_pos.x);
    }
    else if (keycode == 65364)
    {
        // baixo y
        data->light_pos.y = fmod(data->light_pos.y + 1, 200);
        printf("Nova luz.y = %f\n", data->light_pos.y);
    }
    render_scene(data); // redesenha com nova posição
    return 0;
}

int main()
{
    t_data data;
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, 800, 600, "miniRT");
    init_image(&data.img, data.mlx, 800, 600);
    data.light_pos = (t_vector){15, 15, -5};
    data.cam_origin = (t_vector){0, 0, 0};
    data.sphere = (t_sphere){{0, 0, -3}, 1.5, {255, 255, 255}};

    render_scene(&data); // primeira renderização

    mlx_hook(data.win, 2, 1L << 0, key_press, &data); // eventos de tecla
    mlx_loop(data.mlx);
    return 0;
}
