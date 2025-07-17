/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:10:35 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/17 08:55:53 by fjilaias         ###   ########.fr       */
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
		s->center.x -= 0.5;
	else if (keycode == 65363)
		s->center.x += 0.5;
	else if (keycode == 65362)
		s->center.y += 0.5;
	else if (keycode == 65364)
		s->center.y -= 0.5;
	else if (keycode == 'o')
		s->radius += 0.5;
	else if (keycode == 'p')
		s->radius -= 0.5;
	else if (keycode == 'n')
		s->center.z += 0.5;
	else if (keycode == 'u')
		s->center.z -= 0.5;
	else if (keycode == 'n')
		s->center.z += 0.5;
	render_scene(data);
	return (0);
}

void	cylinder_move(int keycode, t_cylinder *c, t_data *data)
{
	if (keycode == 65361)
		c->center.x -= 0.5;
	else if (keycode == 65363)
		c->center.x += 0.5;
	else if (keycode == 65362)
		c->center.y += 0.5;
	else if (keycode == 65364)
		c->center.y -= 0.5;
	else if (keycode == 'o')
		c->height += 0.5;
	else if (keycode == 'p')
		c->height -= 0.5;
	else if (keycode == 'u')
		c->center.z -= 0.5;
	else if (keycode == 'n')
		c->center.z += 0.5;
	else if (keycode == 'x')
		c->normalized = rotate_vector(data, c->normalized, 'x', 0.087);
	else if (keycode == 'y')
		c->normalized = rotate_vector(data, c->normalized, 'y', 0.087);
	else if (keycode == 'z')
		c->normalized = rotate_vector(data, c->normalized, 'z', 0.087);
	c->normalized = vec_normalize(c->normalized);
	print_log(2, data);
	render_scene(data);
}

int	camera_move(int keycode, t_data *data)
{
	if (keycode == 'w')
		data->camera->origin.z += 0.5;
	else if (keycode == 's')
		data->camera->origin.z -= 0.5;
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
		light->position.x -= 0.5;
	else if (keycode == 65363)
		light->position.x += 0.5;
	else if (keycode == 65362)
		light->position.y += 0.5;
	else if (keycode == 65364)
		light->position.y -= 0.5;
	else if (keycode == 'n')
		light->position.z += 0.5;
	else if (keycode == 'u')
		light->position.z -= 0.5;
	data->light_s->center.x = light->position.x;
	data->light_s->center.x = light->position.x;
	data->light_s->center.y = light->position.y;
	data->light_s->center.y = light->position.y;
	data->light_s->center.z = light->position.z;
	data->light_s->center.z = light->position.z;
	render_scene(data);
	return (0);
}
