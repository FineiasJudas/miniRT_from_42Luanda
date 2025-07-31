/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 01:09:35 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/31 12:16:37 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	cylind_size(int keycode, t_cylinder *c)
{
	if (keycode == 'b')
		c->diameter += 0.3;
	else if (keycode == 'v' && ((c->diameter - 0.3) >= 0.1))
		c->diameter -= 0.3;
	else if (keycode == 'p')
		c->height += 0.3;
	else if (keycode == 'o' && ((c->height - 0.3) >= 0.1))
		c->height -= 0.3;
}

void	cylinder_move(int keycode, t_cylinder *c, t_data *data)
{
	if (keycode == 65361)
		c->center.x -= 0.3;
	else if (keycode == 65363)
		c->center.x += 0.3;
	else if (keycode == 65362)
		c->center.y += 0.3;
	else if (keycode == 65364)
		c->center.y -= 0.3;
	else if (keycode == 'u')
		c->center.z -= 0.5;
	else if (keycode == 'n')
		c->center.z += 0.5;
	else if (keycode == 'b' || keycode == 'v' || keycode == 'p'
		|| keycode == 'o')
		cylind_size(keycode, c);
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
