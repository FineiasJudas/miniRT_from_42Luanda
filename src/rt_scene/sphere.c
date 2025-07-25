/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:42:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/25 21:35:42 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	shadow_spheres_check(t_render *render, t_data *data)
{
	float	t_sphere_shadow;
	int		in_shadow;
	float	dist_light;

	in_shadow = 0;
	dist_light = vec_dist(data->light->position, render->hit);
	data->shadow_ray = (t_ray){vec_add(render->hit, vec_scale(render->normal,
				0.001)), vec_normalize(vec_sub(data->light->position,
				render->hit))};
	data->tmp = data->sphere_l;
	while (data->tmp)
	{
		data->s = (t_sphere *)data->tmp->content;
		if (intersect_ray_sphere(data->shadow_ray, data->s, &t_sphere_shadow)
			&& t_sphere_shadow > EPS && t_sphere_shadow < dist_light)
		{
			in_shadow = 1;
			break ;
		}
		data->tmp = data->tmp->next;
	}
	return (in_shadow);
}

int	sphere_shadow_check(t_render *render, t_data *data)
{
	float	diffuse_intensity;
	int		in_shadow;

	in_shadow = 0;
	if (shadow_planes_check(render, data))
		in_shadow = 1;
	if (shadow_spheres_check(render, data))
		in_shadow = 1;
	if (shadow_cylinders_check(render, data))
		in_shadow = 1;
	if (in_shadow)
		diffuse_intensity = 0.0;
	else
		diffuse_intensity = fmax(0.0, vec_dot(render->normal,
					render->light_dir)) * data->light->brightness;
	render->color = ambient_light(&data->sphere->color, diffuse_intensity,
			data->ambient);
	return (0);
}

int	intersect_ray_sphere(t_ray ray, t_sphere *s, float *t)
{
	float		a;
	float		b;
	float		c;
	float		disc;
	t_vector	oc;

	oc = vec_sub(ray.origin, s->center);
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0 * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - s->radius * s->radius;
	disc = b * b - 4 * a * c;
	if (disc < 0)
		return (0);
	*t = (-b - sqrtf(disc)) / (2.0 * a);
	return (*t > 0);
}
