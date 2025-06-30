/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:54:01 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 07:55:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
static float angle = 0.0f;

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

t_vector calculate_cube_normal(t_vector hit, t_cube *cube)
{
    const float epsilon = 0.001f;

    if (fabs(hit.x - cube->min.x) < epsilon)
        return (t_vector){-1, 0, 0};
    else if (fabs(hit.x - cube->max.x) < epsilon)
        return (t_vector){1, 0, 0};
    else if (fabs(hit.y - cube->min.y) < epsilon)
        return (t_vector){0, -1, 0};
    else if (fabs(hit.y - cube->max.y) < epsilon)
        return (t_vector){0, 1, 0};
    else if (fabs(hit.z - cube->min.z) < epsilon)
        return (t_vector){0, 0, -1};
    else if (fabs(hit.z - cube->max.z) < epsilon)
        return (t_vector){0, 0, 1};

    // Caso raro: ponto no meio de uma aresta — escolhe normal nula (ou trata de outra forma)
    return (t_vector){0, 0, 0};
}

int intersect_ray_cube(t_ray ray, t_cube *cube, float *t)
{
    float tmin = (cube->min.x - ray.origin.x) / ray.direction.x;
    float tmax = (cube->max.x - ray.origin.x) / ray.direction.x;
    if (tmin > tmax)
    {
        float tmp = tmin;
        tmin = tmax;
        tmax = tmp;
    }

    float tymin = (cube->min.y - ray.origin.y) / ray.direction.y;
    float tymax = (cube->max.y - ray.origin.y) / ray.direction.y;
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

    float tzmin = (cube->min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (cube->max.z - ray.origin.z) / ray.direction.z;
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

void render_scene_cube(t_data *data)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
            t_ray ray = {data->camera->origin, ray_dir};
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

                t_vector light_dir = vec_normalize(vec_sub(data->light->position, hit));
                float intensity = fmax(0.1, vec_dot(normal, light_dir));
                t_color color = scale_color(data->cube->color, intensity);
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