/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:26:54 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/01 11:52:16 by fjilaias         ###   ########.fr       */
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
    int in_shadow = 0;

    // Raio de sombra com pequeno offset na direção da normal
    t_ray shadow_ray;
    shadow_ray.origin = vec_add(render->hit, vec_scale(render->normal, 0.001));
    shadow_ray.direction = render->light_dir;
    // Verifica interseção com esfera
    t_list *node = data->sphere_l;
    while (node)
    {
        t_sphere *s = (t_sphere *)node->content;
        if (intersect_ray_sphere(shadow_ray, s, &t_sphere_shadow) &&
            t_sphere_shadow < vec_dot(vec_sub(data->light->position, render->hit),
                vec_sub(data->light->position, render->hit)))
            in_shadow = 1;
        node = node->next;
    }
    return (in_shadow);
}

int shadow_plane_check(t_render *render, t_data *data)
{
    float t_plane_shadow;
    float t_shadow;
    int in_shadow = 0;

    // Raio de sombra com pequeno offset na direção da normal
    t_ray shadow_ray;
    shadow_ray.origin = vec_add(render->hit, vec_scale(render->normal, 0.001));
    shadow_ray.direction = render->light_dir;

    t_plane_shadow = intersect_ray_plane(&shadow_ray.origin, &shadow_ray.direction, data->plane);
    if (intersect_ray_sphere(shadow_ray, data->sphere, &t_shadow) && t_shadow 
        < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    else if (t_plane_shadow > 0 && t_plane_shadow < 
        vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    return (in_shadow);
}

int shadow_cylinders_check(t_render *render, t_data *data)
{
    t_list  *node;
    int in_shadow = 0;

    float dist_light = vec_dist(data->light->position, render->hit);
    // Raio de sombra com pequeno offset na direção da normal
    t_ray shadow_ray;
    shadow_ray.origin = vec_add(render->hit, vec_scale(render->normal, 0.001));
    shadow_ray.direction = render->light_dir;

    // Verifica interseção com esfera
    node = data->cylinder_l;
    while (node)
    {
        t_cylinder *c = (t_cylinder *)node->content;
        if (intersect_cylinder(shadow_ray, *c, &c->tc)
            && c->tc > EPS && c->tc < dist_light)
            in_shadow = 1;
        node = node->next;
    }
    return (in_shadow);
}

