/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:47:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/16 16:52:31 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_blank_line(const char *s)
{
	while (*s)
	{
		if (*s != ' ' && *s != '\t' && *s != '\r' && *s != '\n')
			return (0);
		++s;
	}
	return (1);
}

int	render_element(t_render *render, t_data *data, t_object_type type,
		float element)
{
	render->hit = vec_add(data->ray.origin, vec_scale(data->ray.direction,
				element));
	if (type == SPHERE)
	{
		render->normal = vec_normalize(vec_sub(render->hit,
					data->sphere->center));
		render->light_dir = vec_normalize(vec_sub(data->light->position,
					render->hit));
		sphere_shadow_check(render, data);
	}
	else if (type == PLANE)
	{
		render->normal = data->plane->normalized;
		render->light_dir = vec_normalize(vec_sub(data->light->position,
					render->hit));
		plane_shadow_check(render, data);
	}
	else if (type == CYLINDER)
	{
		cylinder_normal(render, data, element);
		cylinder_shadow_check(render, data);
	}
	return (1);
}

void	render_scene(t_data *data)
{
	int	xy[2];

	xy[0] = 0;
	while (xy[0]++ < HEIGHT)
	{
		xy[1] = 0;
		while (xy[1]++ < WIDTH)
		{
			get_ray_direction(xy, WIDTH, HEIGHT, data);
			if (intersect_ray_object(data))
				put_pixel(&data->img, xy[1], xy[0], data->render->color);
			else
				put_pixel(&data->img, xy[1], xy[0], (t_color){19, 24, 33});
		}
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}
