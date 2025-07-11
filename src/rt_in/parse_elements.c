/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 10:11:18 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/10 13:34:19 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int parse_plane(char **tokens, t_data *plane)
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
    return (1);
}

int parse_sphere(char **tokens, t_data *scene)
{
    if (count_tokens(tokens) != 4)
        printf("Formato inválido para esfera");
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (0);
    scene->sphere = (t_sphere *)malloc(sizeof(t_sphere));
    if (!scene->sphere)
        return (0);
    collect_mem(scene->sphere);
    scene->sphere->center =  conv_vector(tokens[1]);
    scene->sphere->radius = float_convert(tokens[2]) / 2.0;
    scene->sphere->color = conv_color(tokens[3]);
    add_to_scene_list(&scene->sphere_l, scene->sphere);
    return (1);
}

int parse_ambient(char **tokens, t_data *a)
{
    if (!tokens[1] || !tokens[2])
        return (0);
    a->ambient = (t_ambient *)malloc(sizeof(t_ambient));
    if (!a->ambient)
        return (0);
    collect_mem(a->ambient);
    a->ambient->ratio = float_convert(tokens[1]);
   // printf("Luz ambiente %f\n",  a->ambient->ratio);
    a->ambient->color = conv_color(tokens[2]);
    return (1);
}

int parse_cylinder(char **tokens, t_data *cylinder)
{
    if (!tokens[1] || !tokens[2] || !tokens[3]
            || !tokens[4] || !tokens[5])
        return (0);
    cylinder->cylinder = (t_cylinder *)malloc(sizeof(t_cylinder));
    if (!cylinder->cylinder)
        return (0);
    collect_mem(cylinder->cylinder);
    cylinder->cylinder->center = conv_vector(tokens[1]);
    cylinder->cylinder->normalized = vec_normalize(conv_vector(tokens[2]));
    cylinder->cylinder->diameter = ft_atoi(tokens[3]);
    cylinder->cylinder->height = ft_atoi(tokens[4]);
    cylinder->cylinder->color = conv_color(tokens[5]);
    add_to_scene_list(&cylinder->cylinder_l, cylinder->cylinder);
    return (1);
}
int parse_light(char **tokens, t_data *light)
{
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (0);
    light->light = (t_light *)malloc(sizeof(t_light));
    if (!light->light)
        return (0);
    collect_mem(light->light);
    light->light->position = conv_vector(tokens[1]);
    light->light->brightness = float_convert(tokens[2]);
    light->light->color = conv_color(tokens[3]);
    return (1);
}

int parse_camera(char **tokens, t_data *scene)
{
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (0);
    scene->camera = (t_camera *)malloc(sizeof(t_camera));
    if (!scene->camera)
        return (0);
    collect_mem(scene->camera);
    scene->camera->direction = vec_normalize(conv_vector(tokens[1]));
    if (scene->camera->direction.x == 0 && scene->camera->direction.y == 0 && scene->camera->direction.z == 0)
    {
        printf("Direção da câmera inválida\n");
        return (0); 
    }
    if (scene->camera->direction.z < 0)
    {
        scene->camera->direction.x *= -1;
        scene->camera->direction.y *= -1;
        scene->camera->direction.z *= -1;
    }
    scene->camera->direction = conv_vector(tokens[1]);
    scene->camera->origin = conv_vector(tokens[2]);
    scene->camera->fov = ft_atoi(tokens[3]);
    return (1);
}
