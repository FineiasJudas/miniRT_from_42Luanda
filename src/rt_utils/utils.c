/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:26:36 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/15 13:35:25 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	rotate_vector(t_vector v, char axis, double angle)
{
	t_vector	r;
	double		cos_a;
	double		sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	if (axis == 'x')
	{
		r.x = v.x;
		r.y = v.y * cos_a - v.z * sin_a;
		r.z = v.y * sin_a + v.z * cos_a;
	}
	else if (axis == 'y')
	{
		r.x = v.x * cos_a + v.z * sin_a;
		r.y = v.y;
		r.z = -v.x * sin_a + v.z * cos_a;
	}
	else if (axis == 'z')
	{
		r.x = v.x * cos_a - v.y * sin_a;
		r.y = v.x * sin_a + v.y * cos_a;
		r.z = v.z;
	}
	return (r);
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
