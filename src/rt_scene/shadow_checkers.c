/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:26:54 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/01 12:54:23 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// utilitário: distância ponto-a-ponto
static float vec_dist(t_vector a, t_vector b)
{
    t_vector d = { a.x-b.x, a.y-b.y, a.z-b.z };
    return sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
}

int shadow_spheres_check(t_render *render, t_data *data)
{
    float t_sphere_shadow;
    int in_shadow;

    // Raio de sombra com pequeno offset na direção da normal
    in_shadow = 0;
    data->shadow_ray = (t_ray){vec_add(render->hit, 
        vec_scale(render->normal, 0.001)), render->light_dir};
    // Verifica interseção com esfera
    data->tmp = data->sphere_l;
    while (data->tmp)
    {
        data->s = (t_sphere *)data->tmp->content;
        if (intersect_ray_sphere(data->shadow_ray, data->s, &t_sphere_shadow) &&
            t_sphere_shadow < vec_dot(vec_sub(data->light->position, render->hit),
                vec_sub(data->light->position, render->hit)))
            in_shadow = 1;
        data->tmp = data->tmp->next;
    }
    return (in_shadow);
}

int shadow_plane_check(t_render *render, t_data *data)
{
    float t_plane_shadow;
    float t_shadow;
    int in_shadow;

    // Raio de sombra com pequeno offset na direção da normal
    in_shadow = 0;
    data->shadow_ray = (t_ray){vec_add(render->hit, vec_scale(render->normal, 0.001)), render->light_dir};
    t_plane_shadow = intersect_ray_plane(&data->shadow_ray.origin, &data->shadow_ray.direction, data->plane);
    if (intersect_ray_sphere(data->shadow_ray, data->sphere, &t_shadow) && t_shadow 
        < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    else if (t_plane_shadow > 0 && t_plane_shadow < 
        vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    return (in_shadow);
}

int shadow_cylinders_check(t_render *render, t_data *data)
{
    float dist_light;
    int in_shadow;

    in_shadow = 0;
    dist_light = vec_dist(data->light->position, render->hit);
    // Raio de sombra com pequeno offset na direção da normal
    data->shadow_ray = (t_ray){vec_add(render->hit, 
        vec_scale(render->normal, 0.001)), render->light_dir};

    // Verifica interseção com esfera
    data->tmp = data->cylinder_l;
    while (data->tmp)
    {
        data->c = (t_cylinder *)data->tmp->content;
        if (intersect_cylinder(data->shadow_ray, *data->c, &data->c->tc)
            && data->c->tc > EPS && data->c->tc < dist_light)
            in_shadow = 1;
        data->tmp = data->tmp->next;
    }
    return (in_shadow);
}

