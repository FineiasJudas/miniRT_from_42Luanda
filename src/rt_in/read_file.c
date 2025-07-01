/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:53:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/01 08:18:23 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
 
int ft_strcmp(const char *s1, const char *s2)
{
    unsigned int i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i ++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int parse_plane(char **tokens, t_data *plane)
{
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (0);
    plane->plane = (t_plane *)malloc(sizeof(t_plane));
    if (!plane->plane)
        return (0);
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
    a->ambient->ratio = float_convert(tokens[1]);
    printf("Luz ambiente %f\n",  a->ambient->ratio);
    a->ambient->color = conv_color(tokens[2]);
    return (1);
}

int parse_cylinder(char **tokens, t_data *cylinder)
{
    if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
        return (0);
    cylinder->cylinder = (t_cylinder *)malloc(sizeof(t_cylinder));
    if (!cylinder)
        return (0);
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
    scene->camera->direction = conv_vector(tokens[1]);
    scene->camera->origin = conv_vector(tokens[2]);
    scene->camera->fov = ft_atoi(tokens[3]);
    return (1);
}

int identify_and_process(char **tokens, t_data *data)
{
    int result;

    if (!tokens || !tokens[0])
        return (1);
    result = 0;
    if (ft_strcmp(tokens[0], "A") == 0)
        result = parse_ambient(tokens, data);
    else if (ft_strcmp(tokens[0], "C") == 0)
        result = parse_camera(tokens, data);
    else if (ft_strcmp(tokens[0], "L") == 0)
        result = parse_light(tokens, data);
    else if (ft_strcmp(tokens[0], "sp") == 0)
        result = parse_sphere(tokens, data);
    else if (ft_strcmp(tokens[0], "pl") == 0)
        result = parse_plane(tokens, data);
    else if (ft_strcmp(tokens[0], "cy") == 0)
        result = parse_cylinder(tokens, data);
    return (result);
}

int process_line(char *line, t_data *data)
{
    char **tokens;
    int result;

    if (!line || ft_strlen(line) < 1)
        return((0 * printf("Sem tokens!\n")) + 1);
    tokens = ft_split(line, ' ');     
    int i = 0;
    while (tokens[i])
        printf("%s\n", tokens[i++]);
    if (!tokens)
        return (1);
    result = identify_and_process(tokens, data);
    // free_tokens(tokens);
    return (result);
}

int read_and_process_lines(int fd, t_data * data)
{
    char    *line;
    int result;

    result = 0;
    while (1)
    {
        line = get_next_line(fd);
        if (line)
        {
            result += process_line(line, data);
            if (line)
                free(line);
        }
        else
            break;
        if (result == 9)
            return (0);
    }
    return (1);
}

int open_rt_file(char *filename)
{
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);
    return (fd);
}

int parse_rt_file(char *filename, t_data *data)
{
    int fd;
    int result;

    fd = open_rt_file(filename);
    if (fd < 0)
        return (1);
    result = read_and_process_lines(fd, data);
    close(fd);
    return (result);
}
void    prin_data(t_data *arg)
{
    if (!arg)
        return ;
    if (arg->sphere)
    {
        printf("\nsphere\nVetor centro: x - %f, y - %f, z - %f\n", arg->sphere->center.x, arg->sphere->center.y, arg->sphere->center.y);
        printf("Radius: - %f\n", arg->sphere->radius);
        printf("Cor r: - %d, g - %d, b - %d\n", arg->sphere->color.r, arg->sphere->color.g, arg->sphere->color.b);
    }
    if (arg->plane)
    {
        printf("\nplane\nCoordenadas do Vetor: x - %f, y - %f, z - %f\n", arg->plane->coordinates.x, arg->plane->coordinates.y, arg->plane->coordinates.y);
        printf("Vetor normalizado centro: x - %f, y - %f, z - %f\n", arg->plane->normalized.x, arg->plane->normalized.y, arg->plane->normalized.y);
        printf("Cor r: - %d, g - %d, b - %d\n", arg->plane->color.r, arg->plane->color.g, arg->plane->color.b);
    }
    if (arg->ambient)
    {
        (printf("\nambient\nRatio do ambiente: r - %f\n", arg->ambient->ratio));
        (printf("Cor da luz ambiente: r - %d, g - %d, b - %d\n", arg->ambient->color.r,  arg->ambient->color.g,  arg->ambient->color.b));
    }
    if (arg->cylinder)
    {
        printf("\ncylinder\nVetor centro: x - %f, y - %f, z - %f\n", arg->cylinder->center.x, arg->cylinder->center.y, arg->cylinder->center.y);
        printf("Vetor normalizado centro: x - %f, y - %f, z - %f\n", arg->cylinder->normalized.x, arg->cylinder->normalized.y, arg->cylinder->normalized.y);
        printf("Diâmetro:  %f\n", arg->cylinder->diameter);
        printf("Altura:  %f\n", arg->cylinder->height);
        printf("Cor da luz ambiente: - %d, g - %d, b - %d\n", arg->cylinder->color.r,  arg->cylinder->color.g,  arg->cylinder->color.b);
    }
    if (arg->light)
    {
        (printf("\nlight\nPosição da luz, x - %f, y - %f, z - %f\n", arg->light->position.x,  arg->light->position.y,  arg->light->position.z));
        (printf("Brilho - %f\n", arg->light->brightness));
        (printf("Cor da luz: r - %d, g - %d, b - %d\n", arg->light->color.r,  arg->light->color.g,  arg->light->color.b));
    }
    if (arg->camera)
    {
        (printf("\ncamera\nDireção da Câmera: x - %f, y - %f, z - %f\n", arg->camera->direction.x,  arg->camera->direction.y,  arg->camera->direction.z));
        (printf("Origem da Câmera: x - %f, y - %f, z - %f\n", arg->camera->origin.x,  arg->camera->origin.y,  arg->camera->origin.z));
        (printf("Cor da luz r: - %f\n", arg->camera->fov));
    }
}
