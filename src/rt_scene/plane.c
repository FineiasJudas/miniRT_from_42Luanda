/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:42:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 15:06:49 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// utilitário: distância ponto-a-ponto
static float vec_dist(t_vector a, t_vector b)
{
    t_vector d = { a.x-b.x, a.y-b.y, a.z-b.z };
    return sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
}

void plane_shadow_check(t_render *render, t_data *data)
{
    int   in_shadow = 0;
    float dist_light;
   // float t_sph;
    double t_cyl;

    // 1) cria shadow‐ray com offset acima do plano
    t_ray shadow_ray;
    shadow_ray.origin    = vec_add(
                              render->hit,
                              vec_scale(data->plane->normalized, EPS)
                           );
    shadow_ray.direction = render->light_dir;

    // 2) distância real até a luz
    dist_light = vec_dist(data->light->position, render->hit);
    // 3) testa esfera
    t_list *node = data->sphere_l;
    while (node)
    {
        t_sphere *t = (t_sphere *)node->content;
        if (intersect_ray_sphere(shadow_ray, t, &t->ts) 
            && t->ts > EPS && t->ts < dist_light)
            in_shadow = 1;
        node = node->next;
    }
    
    // 4) testa cilindro
    if (intersect_cylinder(shadow_ray, *data->cylinder, &t_cyl)
        && t_cyl > EPS
        && t_cyl < dist_light)
        in_shadow = 1;

    // 5) calcula componente difusa
    float diff = in_shadow
                ? 0.0f
                : fmaxf(0.0f,
                        vec_dot(data->plane->normalized,
                                render->light_dir));

    // 6) cor final: ambiente + difusa
    render->color = ambient_light(
                        &data->plane->color,
                        diff,
                        data->ambient
                    );
}

double intersect_ray_plane(t_vector *ray_origin, t_vector *ray_dir, t_plane *plane)
{
    t_vector origin_to_plane = vec_sub(*ray_origin, plane->coordinates);
    double numerator = vec_dot(plane->normalized, origin_to_plane);
    double denominator = vec_dot(plane->normalized, *ray_dir);
    if (fabs(denominator) < 0.0001)
        return -1.0;
    double t = -numerator / denominator;
    if (t > 0.0001)
        return t;
    return -1.0;
}