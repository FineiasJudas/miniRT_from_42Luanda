/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 10:11:18 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/31 12:26:27 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_plane(char **tokens, t_data *plane)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (0);
	plane->plane = (t_plane *)malloc(sizeof(t_plane));
	if (!plane->plane)
		return (0);
	collect_mem(plane->plane);
	plane->plane->coordinates = conv_vector(tokens[1]);
	plane->plane->normalized = vec_normalize(conv_vector(tokens[2]));
	plane->plane->color = conv_color(tokens[3]);
	add_to_scene_list(&plane->plane_l, plane->plane);
	return (1);
}

int	parse_sphere(char **tokens, t_data *scene)
{
	if (count_tokens(tokens) != 4)
		return (0 * printf("Error\nFormato inválido para esfera"));
	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (0);
	scene->sphere = (t_sphere *)malloc(sizeof(t_sphere));
	if (!scene->sphere)
		return (0);
	collect_mem(scene->sphere);
	scene->sphere->center = conv_vector(tokens[1]);
	scene->sphere->radius = my_strtod(tokens[2], &scene->endptr) / 2.0;
	scene->sphere->color = conv_color(tokens[3]);
	add_to_scene_list(&scene->sphere_l, scene->sphere);
	return (1);
}

int	parse_ambient(char **tokens, t_data *a)
{
	if (!tokens[1] || !tokens[2])
		return (0);
	a->ambient = (t_ambient *)malloc(sizeof(t_ambient));
	if (!a->ambient)
		return (0);
	collect_mem(a->ambient);
	a->ambient->ratio = my_strtod(tokens[1], &a->endptr);
	a->ambient->color = conv_color(tokens[2]);
	return (1);
}

int	parse_cylinder(char **tokens, t_data *cylinder)
{
	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		return (0);
	cylinder->cylinder = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!cylinder->cylinder)
		return (0);
	collect_mem(cylinder->cylinder);
	cylinder->cylinder->center = conv_vector(tokens[1]);
	cylinder->cylinder->normalized = vec_normalize(conv_vector(tokens[2]));
	cylinder->cylinder->diameter = my_strtod(tokens[3], &cylinder->endptr);
	cylinder->cylinder->height = my_strtod(tokens[4], &cylinder->endptr);
	cylinder->cylinder->color = conv_color(tokens[5]);
	add_to_scene_list(&cylinder->cylinder_l, cylinder->cylinder);
	return (1);
}

int	parse_light(char **tokens, t_data *light)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (0);
	light->light = (t_light *)malloc(sizeof(t_light));
	light->light_s = (t_sphere *)malloc(sizeof(t_sphere));
	if (!light->light || !light->light_s)
		return (0);
	collect_mem(light->light);
	collect_mem(light->light_s);
	light->light->position = conv_vector(tokens[1]);
	light->light->brightness = my_strtod(tokens[2], &light->endptr);
	light->light->color = conv_color("255,255,255");
	light->light_s->center = light->light->position;
	light->light_s->radius = 0.1;
	light->light_s->color = light->light->color;
	add_to_scene_list(&light->lights_l, light->light);
	return (1);
}
