/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:52:28 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/11 12:59:14 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int get_ray_direction(int x, int y, int width, int height, t_data *data)
{
    double aspect_ratio = (double)width / height;
    double fov_radians = data->camera->fov * M_PI / 180.0;
    double scale = tan(fov_radians / 2.0);

    // Coordenadas normalizadas da tela (px, py) de -1 a +1
    double px = (2.0 * ((double)x + 0.5) / width - 1.0) * aspect_ratio * scale;
    double py = (1.0 - 2.0 * ((double)y + 0.5) / height) * scale;

    // Base da câmera
    t_vector forward = vec_normalize(data->camera->direction);
    t_vector world_up = {0, 1, 0};
    t_vector right = vec_normalize(cross(forward, world_up));
    t_vector up = vec_normalize(cross(right, forward));

    // Direção do raio combinando os vetores
    t_vector ray_direction = vec_add(
        vec_add(vec_scale(right, px), vec_scale(up, py)),
        forward
    );

    data->ray.origin = data->camera->origin;
    data->ray.direction = vec_normalize(ray_direction);
    return 0;
}


/*int get_ray_direction(int x, int y, int width, int height, t_data *data)
{
    double  aspect_ratio;

    aspect_ratio = (double)width / height;
    data->ray.direction.x = (2.0 * x / width - 1.0) * aspect_ratio; // Normaliza x para [-aspect, aspect]
    data->ray.direction.y = 1.0 - 2.0 * y / height;      // Normaliza y para [-1, 1]
    data->ray.direction.z = -1.0;                           // Direção Z (para frente).
    data->ray.origin = data->camera->origin;
    return (0);
}*/

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
