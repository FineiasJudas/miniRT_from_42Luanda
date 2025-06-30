/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:52:28 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 07:52:52 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void get_ray_direction(int x, int y, int width, int height, t_data *data)
{
    double  aspect_ratio;

    aspect_ratio = (double)width / height;
    data->ray.direction.x = (2.0 * x / width - 1.0) * aspect_ratio; // Normaliza x para [-aspect, aspect]
    data->ray.direction.y = 1.0 - 2.0 * y / height;      // Normaliza y para [-1, 1]
    data->ray.direction.z = -1.0;                           // Direção Z (para frente).
    data->ray.origin = data->camera->origin;
}

t_color calc_ambient(t_color *c, t_ambient *ambient)
{
    t_color out;

    out.r =(int) c->r * (ambient->color.r / 255.0) * ambient->ratio;
    out.g =(int) c->g * (ambient->color.g / 255.0) * ambient->ratio;
    out.b =(int) c->b * (ambient->color.b / 255.0) * ambient->ratio;
    
    if (out.r > 255) out.r = 255;
    if (out.g > 255) out.g = 255;
    if (out.b > 255) out.b = 255;
    if (out.r < 0) out.r = 0;
    if (out.g < 0) out.g = 0;
    if (out.b < 0) out.b = 0;

    return (out);
}

t_color ambient_light(t_color *src, double intensity, t_ambient *ambient)
{
    t_color dif;
    t_color amb;
    t_color out;

    dif = scale_color(*src, intensity);
    amb = calc_ambient(src, ambient);

    out.r = dif.r + amb.r;
    out.g = dif.g + amb.g;
    out.b = dif.b + amb.b;
    if (out.r > 255) out.r = 255;
    if (out.g > 255) out.g = 255;
    if (out.b > 255) out.b = 255;
    return (out);
}