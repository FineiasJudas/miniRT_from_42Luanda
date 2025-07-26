/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:03 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/26 10:50:45 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	cylinder_normal(t_render *render, t_data *data)
{
	t_vector	d_c_h[3];
	double		halfh;
	t_vector	ch;
	double		y;
	t_vector	proj;

	d_c_h[1] = data->cylinder->normalized;
	d_c_h[2] = data->cylinder->center;
	halfh = data->cylinder->height / 2.0;
	ch = vec_sub(render->hit, d_c_h[2]);
	y = vec_dot(ch, d_c_h[1]);
	if (fabs(y) > halfh - EPS && fabs(y) < halfh + EPS)
	{
		if (y > 0)
			render->normal = d_c_h[1];
		else
			render->normal = vec_scale(d_c_h[1], -1.0);
	}
	else
	{
		proj = vec_scale(d_c_h[1], y);
		render->normal = vec_normalize(vec_sub(ch, proj));
	}
	if (vec_dot(render->normal, render->light_dir) < 0)
		render->normal = vec_scale(render->normal, -1.0);
}

int	shadow_cylinders_check(t_render *render, t_data *data)
{
	float	dist_light;
	int		in_shadow;

	in_shadow = 0;
	dist_light = vec_dist(data->light->position, render->hit);
	data->bias_offset = vec_scale(render->light_dir, data->bias);
	data->shadow_ray.origin = vec_add(render->hit, data->bias_offset);
	data->shadow_ray.direction = vec_normalize(render->light_dir);
	data->tmp = data->cylinder_l;
	while (data->tmp)
	{
		data->c = (t_cylinder *)data->tmp->content;
		if (intersect_cylinder(data->shadow_ray, *data->c, &data->c->tc)
			&& data->c->tc > data->bias && data->c->tc < dist_light)
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
					render->light_dir)) * data->light->brightness;
	render->color = ambient_light(&data->cylinder->color, diffuse_intensity,
			data->ambient);
	return (0);
}
