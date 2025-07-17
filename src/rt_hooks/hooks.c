/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:41:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/16 11:24:03 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_selected_object(t_data *data, t_object_type type, t_list *node)
{
	data->selected_type = type;
	data->selected_node = node;
	if (node)
		data->selected_obj = node->content;
	else
		data->selected_obj = NULL;
}

static void	select_next_type(t_data *data)
{
	if (data->selected_type == SPHERE)
		set_selected_object(data, CYLINDER, data->cylinder_l);
	else if (data->selected_type == CYLINDER)
		set_selected_object(data, PLANE, data->plane_l);
	else if (data->selected_type == PLANE)
		set_selected_object(data, LIGHT, data->lights_l);
	else
		set_selected_object(data, SPHERE, data->sphere_l);
}

void	select_next_object(t_data *data)
{
	if (data->selected_node && data->selected_node->next)
		set_selected_object(data, data->selected_type,
			data->selected_node->next);
	else
		select_next_type(data);
	if (!data->selected_node)
	{
		data->selected_type = NONE;
		data->selected_obj = NULL;
		printf("Nada selecionado\n");
		return ;
	}
	if (data->selected_type == SPHERE)
		printf("Selecionou esfera\n");
	else if (data->selected_type == CYLINDER)
		printf("Selecionou cilindro\n");
	else if (data->selected_type == PLANE)
		printf("Selecionou plano\n");
	else if (data->selected_type == LIGHT)
		printf("Selecionou luz\n");
}

int	is_any_object_selected(t_data *data)
{
	return (data->selected_type != NONE && data->selected_obj != NULL);
}

int	key_press(int keycode, t_data *data)
{
	if (keycode == 65307)
		close_window(data);
	else if (keycode == 65289)
		select_next_object(data);
	else if (any_object_moviment_key(keycode))
	{
		if (!is_any_object_selected(data))
			return (0);
		if (data->selected_type == SPHERE)
			sphere_move(keycode, (t_sphere *)data->selected_obj, data);
		else if (data->selected_type == CYLINDER)
			cylinder_move(keycode, (t_cylinder *)data->selected_obj, data);
		else if (data->selected_type == PLANE)
			plane_move(keycode, (t_plane *)data->selected_obj, data);
		else if (data->selected_type == LIGHT)
			light_move(keycode, (t_light *)data->selected_obj, data);
	}
	else if (any_camera_moviment_key(keycode))
		camera_move(keycode, data);
	return (0);
}
