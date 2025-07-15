/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:52:28 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/15 13:15:27 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// utilitário: distância ponto-a-ponto
float	vec_dist(t_vector a, t_vector b)
{
	t_vector	d;

	d.x = a.x - b.x;
	d.y = a.y - b.y;
	d.z = a.z - b.z;
	return (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z));
}

int	get_ray_direction(int *xy, int width, int height, t_data *data)
{
	double		ar_fov_scale[3];
	double		p[2];
	t_vector	forward;
	t_vector	world_up;
	t_vector	right;
	t_vector	up;
	t_vector	ray_direction;

	ar_fov_scale[0] = (double)width / height;
	ar_fov_scale[1] = data->camera->fov * PI / 180.0;
	ar_fov_scale[2] = tan(ar_fov_scale[1] / 2.0);
	p[0] = (2.0 * ((double)xy[1] + 0.5) / width - 1.0) * ar_fov_scale[0]
		* ar_fov_scale[2];
	p[1] = (1.0 - 2.0 * ((double)xy[0] + 0.5) / height) * ar_fov_scale[2];
	forward = vec_normalize(data->camera->direction);
	world_up.x = 0;
	world_up.y = 1;
	world_up.z = 0;
	right = vec_normalize(cross(forward, world_up));
	up = vec_normalize(cross(right, forward));
	ray_direction = vec_add(vec_add(vec_scale(right, p[0]), vec_scale(up,
					p[1])), forward);
	data->ray.origin = data->camera->origin;
	data->ray.direction = vec_normalize(ray_direction);
	return (0);
}

t_color	calc_ambient(t_color *c, t_ambient *ambient)
{
	t_color	out;

	out.r = (int)c->r * (ambient->color.r / 255.0) * ambient->ratio;
	out.g = (int)c->g * (ambient->color.g / 255.0) * ambient->ratio;
	out.b = (int)c->b * (ambient->color.b / 255.0) * ambient->ratio;
	if (out.r > 255)
		out.r = 255;
	if (out.g > 255)
		out.g = 255;
	if (out.b > 255)
		out.b = 255;
	if (out.r < 0)
		out.r = 0;
	if (out.g < 0)
		out.g = 0;
	if (out.b < 0)
		out.b = 0;
	return (out);
}

t_color	ambient_light(t_color *src, double intensity, t_ambient *ambient)
{
	t_color	dif;
	t_color	amb;
	t_color	out;

	dif = scale_color(*src, intensity);
	amb = calc_ambient(src, ambient);
	out.r = dif.r + amb.r;
	out.g = dif.g + amb.g;
	out.b = dif.b + amb.b;
	if (out.r > 255)
		out.r = 255;
	if (out.g > 255)
		out.g = 255;
	if (out.b > 255)
		out.b = 255;
	return (out);
}
