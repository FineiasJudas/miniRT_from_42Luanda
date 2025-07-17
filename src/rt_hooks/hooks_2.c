/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:43:49 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/17 09:09:45 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	close_window(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	if (data->img.img_ptr)
		mlx_destroy_image(data->mlx, data->img.img_ptr);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	free(data->mlx);
	free_list(&data->sphere_l);
	free_list(&data->cylinder_l);
	free_list(&data->lights_l);
	free_list(&data->plane_l);
	free_mem(get_mem_address());
	printf("Janela fechada e memória liberada.\n");
	exit(0);
}

int	resize_window(int width, int height, t_data *data)
{
	if (data->img.img_ptr)
		mlx_destroy_image(data->mlx, data->img.img_ptr);
	init_image(&data->img, data->mlx, width, height);
	mlx_clear_window(data->mlx, data->win);
	render_scene(data);
	return (0);
}

int	any_object_moviment_key(int keycode)
{
	if (keycode == 65361 || keycode == 65363 || keycode == 65362
		|| keycode == 65364 || keycode == 'u' || keycode == 'n'
		|| keycode == 'x' || keycode == 'y' || keycode == 'z' || keycode == 'o'
		|| keycode == 'p')
		return (1);
	return (0);
}

int	any_camera_moviment_key(int keycode)
{
	if (keycode == 119 || keycode == 115 || keycode == 97 || keycode == 100
		|| keycode == 113 || keycode == 101 || keycode == 'j' || keycode == 'l'
		|| keycode == 'i' || keycode == 'k')
		return (1);
	return (0);
}
