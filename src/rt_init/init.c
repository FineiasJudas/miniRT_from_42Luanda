/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:48:53 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/26 02:56:32 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	aux_create_data(t_data *data)
{
	data->ambient = NULL;
	data->cam_origin = NULL;
	data->camera = NULL;
	data->cylinder = NULL;
	data->light = NULL;
	data->mlx = NULL;
	data->plane = NULL;
	data->render = NULL;
	data->sphere = NULL;
	data->win = NULL;
	data->img.addr = NULL;
	data->img.img_ptr = NULL;
	data->img.bits_per_pixel = 0;
	data->img.line_length = 0;
	data->img.endian = 0;
	data->img.width = 0;
	data->img.height = 0;
	data->invalid_line = 0;
	data->matrix = NULL;
	data->bias = (float)(0.00100000004749745131F);
}

void	create_data(t_data *data)
{
	aux_create_data(data);
	data->filename = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->fd = -1;
	data->img.img_ptr = NULL;
	data->img.addr = NULL;
	data->img.bits_per_pixel = 0;
	data->img.line_length = 0;
	data->img.endian = 0;
	data->img.width = 0;
	data->img.height = 0;
	data->invalid_line = 0;
	data->hit_type = -1;
	data->selected_type = NONE;
	data->selected_node = NULL;
	data->selected_obj = NULL;
	data->sphere_l = NULL;
	data->cylinder_l = NULL;
	data->lights_l = NULL;
	data->light_s = NULL;
	data->plane_l = NULL;
	data->tmp = NULL;
	data->c = NULL;
	data->s = NULL;
}

void	init_image(t_image *img, void *mlx, int width, int height)
{
	img->width = width;
	img->height = height;
	img->img_ptr = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
			&img->line_length, &img->endian);
}

void	put_pixel(t_image *img, int x, int y, t_color color)
{
	char	*dst;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color_to_int(color);
}
