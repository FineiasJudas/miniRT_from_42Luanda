/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:42:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/26 10:54:51 by fjilaias         ###   ########.fr       */
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
	data->bias_offset = vec_scale(render->light_dir, data->bias);
	data->shadow_ray.origin = vec_add(render->hit, data->bias_offset);
	data->shadow_ray.direction = vec_normalize(render->light_dir);
	data->tmp = data->sphere_l;
	while (data->tmp)
	{
		data->s = (t_sphere *)data->tmp->content;
		if (intersect_ray_sphere(data->shadow_ray, data->s, &t_sphere_shadow)
			&& t_sphere_shadow > data->bias && t_sphere_shadow < dist_light)
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
	t_vector	oc;
	float		a;
	float		b;
	float		c;
	float		disc;

	oc = vec_sub(ray.origin, s->center);
	a = vec_dot(ray.direction, ray.direction);
	b = 2.0f * vec_dot(oc, ray.direction);
	c = vec_dot(oc, oc) - s->radius * s->radius;
	disc = b * b - 4 * a * c;
	if (disc < 0.0f)
		return (0);
	s->sqrt_disc = sqrtf(disc);
	s->t0 = (-b - s->sqrt_disc) / (2.0f * a);
	s->t1 = (-b + s->sqrt_disc) / (2.0f * a);
	if (s->t0 < EPS && s->t1 < EPS)
		return (0);
	if (s->t0 > EPS)
		*t = s->t0;
	else
		*t = s->t1;
	return (1);
}
