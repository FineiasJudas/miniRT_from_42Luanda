/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:26:36 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/17 09:05:57 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	rotate_vector(t_data *data, t_vector v, char axis, double angle)
{
	data->rot.cos_a = cos(angle);
	data->rot.sin_a = sin(angle);
	if (axis == 'x')
	{
		data->rot.r.x = v.x;
		data->rot.r.y = v.y * data->rot.cos_a - v.z * data->rot.sin_a;
		data->rot.r.z = v.y * data->rot.sin_a + v.z * data->rot.cos_a;
	}
	else if (axis == 'y')
	{
		data->rot.r.x = v.x * data->rot.cos_a + v.z * data->rot.sin_a;
		data->rot.r.y = v.y;
		data->rot.r.z = -v.x * data->rot.sin_a + v.z * data->rot.cos_a;
	}
	else if (axis == 'z')
	{
		data->rot.r.x = v.x * data->rot.cos_a - v.y * data->rot.sin_a;
		data->rot.r.y = v.x * data->rot.sin_a + v.y * data->rot.cos_a;
		data->rot.r.z = v.z;
	}
	else
		data->rot.r = (t_vector){0, 0, 0};
	return (data->rot.r);
}

t_vector	look_at(t_vector from, t_vector to)
{
	return (vec_normalize((t_vector){to.x - from.x, to.y - from.y, to.z
			- from.z}));
}

t_vector	cross(t_vector a, t_vector b)
{
	return ((t_vector){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y
		- a.y * b.x});
}

int	color_to_int(t_color color)
{
	return (((color.r & 0xFF) << 16)
		| ((color.g & 0xFF) << 8) | (color.b & 0xFF));
}

int	count_tokens(char **strs)
{
	int	i;

	i = -1;
	while (strs[++i])
		;
	return (i);
}
