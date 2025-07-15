/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:52:28 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/15 15:25:23 by fjilaias         ###   ########.fr       */
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
	t_vector	cor[3];
	double		aspect;
	double		fov_scale;
	double		n_xy[2];
	t_vector	ray_direction;

	aspect = (double)width / height;
	fov_scale = tan(data->camera->fov * PI / 360.0);
	n_xy[0] = (2.0 * ((double)xy[1] + 0.5) / width - 1.0) * aspect * fov_scale;
	n_xy[1] = (1.0 - 2.0 * ((double)xy[0] + 0.5) / height) * fov_scale;
	cor[0] = vec_normalize(data->camera->dir);
	cor[1] = vec_normalize(cross(cor[0], (t_vector){0, 1, 0}));
	cor[2] = vec_normalize(cross(cor[1], cor[0]));
	ray_direction = vec_add(vec_add(vec_scale(cor[1], n_xy[0]),
				vec_scale(cor[2], n_xy[1])), cor[0]);
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

void	print_log(t_object_type type, t_data *data)
{
	if (type == PLANE)
	{
		printf("Plano movido para: (%f, %f, %f)\n", data->p->coordinates.x,
			data->p->coordinates.y, data->p->coordinates.z);
	}
	else if (type == CYLINDER)
	{
		printf("Cilindro em (%f, %f, %f), eixo: (%f, %f, %f)\n",
			data->c->center.x, data->c->center.y, data->c->center.z,
			data->c->normalized.x, data->c->normalized.y,
			data->c->normalized.z);
	}
	else if (type == 3)
	{
		printf("Camera pos: (%.2f, %.2f, %.2f), direção: (%.2f, %.2f, %.2f)\n",
			data->camera->origin.x, data->camera->origin.y,
			data->camera->origin.z, data->camera->dir.x, data->camera->dir.y,
			data->camera->dir.z);
	}
}
