/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:42:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/25 22:09:21 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	shadow_planes_check(t_render *render, t_data *data)
{
	float	plane;
	float	dist_light;
	int		in_shadow;

	in_shadow = 0;
	dist_light = vec_dist(data->light->position, render->hit);
	data->shadow_ray = (t_ray){vec_add(render->hit, vec_scale(render->normal,
				1e-4)), vec_normalize(render->light_dir)};
	data->tmp = data->plane_l;
	while (data->tmp)
	{
		data->p = (t_plane *)data->tmp->content;
		plane = intersect_ray_plane(&data->shadow_ray.origin,
				&data->shadow_ray.direction, data->p);
		if (plane > EPS && plane < dist_light)
		{
			in_shadow = 1;
			break ;
		}
		data->tmp = data->tmp->next;
	}
	return (in_shadow);
}

int	plane_shadow_check(t_render *render, t_data *data)
{
	float	diffuse_intensity;
	int		in_shadow;

	in_shadow = 0;
	if (shadow_spheres_check(render, data))
		in_shadow = 1;
	if (shadow_cylinders_check(render, data))
		in_shadow = 1;
	if (shadow_planes_check(render, data))
		in_shadow = 1;
	if (in_shadow)
		diffuse_intensity = 0.0f;
	else
		diffuse_intensity = fmax(0.0, vec_dot(render->normal,
					render->light_dir)) * data->light->brightness;
	render->color = ambient_light(&data->plane->color, diffuse_intensity,
			data->ambient);
	return (0);
}

double	intersect_ray_plane(t_vector *ray_origin, t_vector *ray_dir,
		t_plane *plane)
{
	t_vector	origin_to_plane;
	double		denominator;
	double		numerator;
	double		t;

	origin_to_plane = vec_sub(*ray_origin, plane->coordinates);
	numerator = vec_dot(plane->normalized, origin_to_plane);
	denominator = vec_dot(plane->normalized, *ray_dir);
	if (fabs(denominator) < 0.0001)
		return (-1.0);
	t = -numerator / denominator;
	if (t > 0.0001)
		return (t);
	return (-1.0);
}
