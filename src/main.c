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

static float angle = 0.0f;

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

/*
INICIO INTERACAO
intercesao dos raios da camera com os objectos
colocar todos num ficheiros ou varios(regra das 5 funcoes)
*/

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

int intersect_ray_cube(t_ray ray, t_cube cube, float *t)
{
    float tmin = (cube.min.x - ray.origin.x) / ray.direction.x;
    float tmax = (cube.max.x - ray.origin.x) / ray.direction.x;
    if (tmin > tmax)
    {
        float tmp = tmin;
        tmin = tmax;
        tmax = tmp;
    }

    float tymin = (cube.min.y - ray.origin.y) / ray.direction.y;
    float tymax = (cube.max.y - ray.origin.y) / ray.direction.y;
    if (tymin > tymax)
    {
        float tmp = tymin;
        tymin = tymax;
        tymax = tmp;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return 0;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (cube.min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (cube.max.z - ray.origin.z) / ray.direction.z;
    if (tzmin > tzmax)
    {
        float tmp = tzmin;
        tzmin = tzmax;
        tzmax = tmp;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return 0;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    if (tmax < 0)
        return 0;

    *t = (tmin > 0) ? tmin : tmax;
    return 1;
}

/*
MOVENDO OBJECTOS
*/

t_vector rotate_y(t_vector v, float angle)
{
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    return (t_vector){
        v.x * cos_a + v.z * sin_a,
        v.y,
        -v.x * sin_a + v.z * cos_a};
}

/*FIM INTERACAO*/

/*INICIO DOS RENDER(DEPOIS REFACTORAR E TORNAR GENERICO)*/
void render_scene(t_data *data)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
            t_ray ray = {data->camera.origin, ray_dir};
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

// CUBO

t_vector calculate_cube_normal(t_vector hit, t_cube cube)
{
    const float epsilon = 0.001f;

    if (fabs(hit.x - cube.min.x) < epsilon)
        return (t_vector){-1, 0, 0};
    else if (fabs(hit.x - cube.max.x) < epsilon)
        return (t_vector){1, 0, 0};
    else if (fabs(hit.y - cube.min.y) < epsilon)
        return (t_vector){0, -1, 0};
    else if (fabs(hit.y - cube.max.y) < epsilon)
        return (t_vector){0, 1, 0};
    else if (fabs(hit.z - cube.min.z) < epsilon)
        return (t_vector){0, 0, -1};
    else if (fabs(hit.z - cube.max.z) < epsilon)
        return (t_vector){0, 0, 1};

    // Caso raro: ponto no meio de uma aresta — escolhe normal nula (ou trata de outra forma)
    return (t_vector){0, 0, 0};
}

void render_scene_cube(t_data *data)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
            t_ray ray = {data->camera.origin, ray_dir};
            float t;

            // rotacionar origem e direção do raio
            angle += 0.01f; // gira um pouco a cada frame

            t_ray transformed_ray;
            (void)transformed_ray;
            transformed_ray.origin = rotate_y(ray.origin, -angle);
            transformed_ray.direction = rotate_y(ray.direction, -angle);

            if (intersect_ray_cube(ray, data->cube, &t))
            {
                t_vector hit = vec_add(ray.origin, vec_scale(ray.direction, t));

                // Calcula normal do cubo(em world space, depois da rotação)
                t_vector local_hit = rotate_y(hit, -angle); // ponto no sistema local

                // Calcular normal baseado na face atingida (simplificado)

                t_vector normal = calculate_cube_normal(local_hit, data->cube);
                normal = rotate_y(normal, angle); // volta a normal pro mundo

                t_vector light_dir = vec_normalize(vec_sub(data->light_pos, hit));
                float intensity = fmax(0.1, vec_dot(normal, light_dir));
                t_color color = scale_color(data->cube.color, intensity);
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
/*FIM RENDER*/

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

    // mudar a posicao da camera com w-a-s
    else if (keycode == 119)
    {
        data->camera.origin.z = fmod(data->camera.origin.z + 1, 50);
    }
    else if (keycode == 97)
    {
        data->camera.origin.y = fmod(data->camera.origin.y + 1, 50);
    }
    else if (keycode == 115)
    {
        data->camera.origin.x = fmod(data->camera.origin.x + 1, 50);
    }
    render_scene_cube(data); // primeira renderização
//    render_scene(data); // redesenha com nova posição
    return 0;
}

int main()
{
    t_data data;
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, 800, 600, "miniRT");
    init_image(&data.img, data.mlx, 800, 600);

    // data.light_pos = (t_vector){2, 4, 0};
    data.light_pos = (t_vector){0, 4, -1};

    data.camera = (t_camera){{0, 0, 0}, {0, 0, 0}, 32};
    data.cam_origin = (t_vector){0, 0, 0}; // nao usar

    data.cube.min = (t_vector){-1, -1, -4};
    data.cube.max = (t_vector){1, 1, -2};
    data.cube.color = (t_color){255, 255, 255};

    data.sphere = (t_sphere){{0, 0, -3}, 1.5, {255, 255, 255}};

    render_scene_cube(&data); // primeira renderização

    // render_scene(&data); // primeira renderização

    mlx_hook(data.win, 2, 1L << 0, key_press, &data); // eventos de tecla
    mlx_loop(data.mlx);
    return 0;
}
