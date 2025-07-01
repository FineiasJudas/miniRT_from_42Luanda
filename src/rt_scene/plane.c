/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:42:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/01 13:59:56 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void plane_shadow_check(t_render *render, t_data *data)
{
    float diff;
    int in_shadow;

    in_shadow = 0;
    if(shadow_spheres_check(render, data))
        in_shadow = 1;
    // Verifica interseção com o próprio cilindro (ou outros, se houver)
    if (shadow_cylinders_check(render, data))
        in_shadow = 1;
    if (in_shadow)
        diff = 0.0f;
    else
        diff = fmax(0.0, vec_dot(data->plane->normalized, render->light_dir));
    // 6) cor final: ambiente + difusa
    render->color = ambient_light(&data->plane->color, 
        diff, data->ambient);
}

double intersect_ray_plane(t_vector *ray_origin, t_vector *ray_dir, t_plane *plane)
{
    t_vector origin_to_plane;
    double denominator;
    double numerator;
    double t;

    origin_to_plane = vec_sub(*ray_origin, plane->coordinates);
    numerator = vec_dot(plane->normalized, origin_to_plane);
    denominator = vec_dot(plane->normalized, *ray_dir);
    if (fabs(denominator) < 0.0001)
        return (-1.0);
    t = -numerator / denominator;
    if (t > 0.0001)
        return (t);
    return (-1.0);
}