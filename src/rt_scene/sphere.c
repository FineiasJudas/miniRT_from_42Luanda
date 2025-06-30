/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:42:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 15:14:21 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int intersect_ray_sphere(t_ray ray, t_sphere *s, float *t)
{
    t_vector oc = vec_sub(ray.origin, s->center);
    float a = vec_dot(ray.direction, ray.direction);
    float b = 2.0 * vec_dot(oc, ray.direction);
    float c = vec_dot(oc, oc) - s->radius * s->radius;
    float disc = b * b - 4 * a * c;

    if (disc < 0)
        return 0;
    *t = (-b - sqrtf(disc)) / (2.0 * a);
    return (*t > 0);
}

void    sphere_shadow_check(t_render *render, t_data *data)
{
    float   t_shadow;
    double  t_plane_shadow;
    double  t_cyl;
    float   diffuse_intensity;
    int in_shadow;

    // Verificar se está na sombra
    in_shadow = 0;
    t_cyl = EPS;

    t_ray shadow_ray = {vec_add(render->hit, vec_scale(render->normal , 0.001)), render->light_dir}; // Pequeno offset para evitar auto-interseção

    t_plane_shadow = intersect_ray_plane(&shadow_ray.origin, &shadow_ray.direction, data->plane);
    if (intersect_ray_sphere(shadow_ray, data->sphere, &t_shadow) && t_shadow 
        < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    else if (t_plane_shadow > 0 && t_plane_shadow < 
        vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    
    if (intersect_cylinder(shadow_ray, *data->cylinder, &t_cyl)
        && t_cyl > EPS
        && t_cyl)
        in_shadow = 1;
    
        // Calcular intensidade

    diffuse_intensity = in_shadow ? 0.0 : fmax(0.0, vec_dot(render->normal, render->light_dir));
    render->color = ambient_light(&data->sphere->color, diffuse_intensity, data->ambient);
}