/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:10:35 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/31 12:18:29 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	plane_move(int keycode, t_plane *p, t_data *data)
{
	if (keycode == 65361)
		p->coordinates.x -= 0.5;
	else if (keycode == 65363)
		p->coordinates.x += 0.5;
	else if (keycode == 65362)
		p->coordinates.y += 0.5;
	else if (keycode == 65364)
		p->coordinates.y -= 0.5;
	else if (keycode == 'u')
		p->coordinates.z -= 0.5;
	else if (keycode == 'n')
		p->coordinates.z += 0.5;
	else if (keycode == 'x')
		p->normalized = rotate_vector(data, p->normalized, 'x', 0.087);
	else if (keycode == 'y')
		p->normalized = rotate_vector(data, p->normalized, 'y', 0.087);
	else if (keycode == 'z')
		p->normalized = rotate_vector(data, p->normalized, 'z', 0.087);
	p->normalized = vec_normalize(p->normalized);
	print_log(1, data);
	render_scene(data);
	return (0);
}

int	sphere_move(int keycode, t_sphere *s, t_data *data)
{
	if (keycode == 65361)
		s->center.x -= 0.3;
	else if (keycode == 65363)
		s->center.x += 0.3;
	else if (keycode == 65362)
		s->center.y += 0.3;
	else if (keycode == 65364)
		s->center.y -= 0.3;
	else if (keycode == 'o' && (s->radius - 0.1) >= 0.1)
		s->radius -= 0.1;
	else if (keycode == 'p')
		s->radius += 0.1;
	else if (keycode == 'n')
		s->center.z += 0.3;
	else if (keycode == 'u')
		s->center.z -= 0.3;
	else if (keycode == 'n')
		s->center.z += 0.3;
	render_scene(data);
	return (0);
}

int	camera_move(int keycode, t_data *data)
{
	if (keycode == 'w')
		data->camera->origin.z -= 0.5;
	else if (keycode == 's')
		data->camera->origin.z += 0.5;
	else if (keycode == 'd')
		data->camera->origin.x += 0.5;
	else if (keycode == 'a')
		data->camera->origin.x -= 0.5;
	else if (keycode == 'e')
		data->camera->origin.y += 0.5;
	else if (keycode == 'q')
		data->camera->origin.y -= 0.5;
	else if (keycode == 'j')
		data->camera->dir = rotate_vector(data, data->camera->dir, 'y', 0.087);
	else if (keycode == 'l')
		data->camera->dir = rotate_vector(data, data->camera->dir, 'y', -0.087);
	else if (keycode == 'i')
		data->camera->dir = rotate_vector(data, data->camera->dir, 'x', 0.087);
	else if (keycode == 'k')
		data->camera->dir = rotate_vector(data, data->camera->dir, 'x', -0.087);
	data->camera->dir = vec_normalize(data->camera->dir);
	print_log(3, data);
	render_scene(data);
	return (0);
}

int	light_move(int keycode, t_light *light, t_data *data)
{
	if (keycode == 65361)
		light->position.x -= 0.3;
	else if (keycode == 65363)
		light->position.x += 0.3;
	else if (keycode == 65362)
		light->position.y += 0.3;
	else if (keycode == 65364)
		light->position.y -= 0.3;
	else if (keycode == 'n')
		light->position.z += 0.3;
	else if (keycode == 'u')
		light->position.z -= 0.3;
	data->light_s->center.x = light->position.x;
	data->light_s->center.x = light->position.x;
	data->light_s->center.y = light->position.y;
	data->light_s->center.y = light->position.y;
	data->light_s->center.z = light->position.z;
	data->light_s->center.z = light->position.z;
	render_scene(data);
	return (0);
}
