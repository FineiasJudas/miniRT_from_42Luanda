/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:03 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/15 11:39:34 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	cylinder_normal(t_render *render, t_data *data, float t)
{
	t_vector	d;
	t_vector	c;
	t_vector	hit;
	double		halfh;
	t_vector	ch;
	double		y;
	t_vector	proj;

	hit = vec_add(data->ray.origin, vec_scale(data->ray.direction, t));
	d = data->cylinder->normalized;
	c = data->cylinder->center;
	halfh = data->cylinder->height / 2.0;
	ch = vec_sub(hit, c);
	y = vec_dot(ch, d);
	if (fabs(y) > halfh - EPS && fabs(y) < halfh + EPS)
	{
		if (y > 0)
			render->normal = d;
		else
			render->normal = vec_scale(d, -1.0);
	}
	else
	{
		proj = vec_scale(d, y);
		render->normal = vec_normalize(vec_sub(ch, proj));
	}
	render->light_dir = vec_normalize(vec_sub(data->light->position, hit));
}

int	shadow_cylinders_check(t_render *render, t_data *data)
{
	float	dist_light;
	int		in_shadow;

	in_shadow = 0;
	dist_light = vec_dist(data->light->position, render->hit);
	data->shadow_ray = (t_ray){vec_add(render->hit, vec_scale(render->normal,
				0.001)), render->light_dir};
	data->tmp = data->cylinder_l;
	while (data->tmp)
	{
		data->c = (t_cylinder *)data->tmp->content;
		if (intersect_cylinder(data->shadow_ray, *data->c, &data->c->tc)
			&& data->c->tc > EPS && data->c->tc < dist_light)
			in_shadow = 1;
		data->tmp = data->tmp->next;
	}
	return (in_shadow);
}

int	cylinder_shadow_check(t_render *render, t_data *data)
{
	float	diffuse_intensity;
	int		in_shadow;

	in_shadow = 0;
	if (shadow_spheres_check(render, data))
		in_shadow = 1;
	if (shadow_planes_check(render, data))
		in_shadow = 1;
	if (shadow_cylinders_check(render, data))
		in_shadow = 1;
	if (in_shadow)
		diffuse_intensity = 0.0;
	else
		diffuse_intensity = fmax(0.0, vec_dot(render->normal,
					render->light_dir));
	render->color = ambient_light(&data->cylinder->color, diffuse_intensity,
			data->ambient);
	return (0);
}
