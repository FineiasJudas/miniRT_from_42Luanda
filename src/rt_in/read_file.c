/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:53:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/10 14:14:45 by fjilaias         ###   ########.fr       */
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
    if (!tokens)
        return (1);
    result = identify_and_process(tokens, data);
    free_tokens(tokens);
    return (result);
}

int read_and_process_lines(int fd, t_data *data)
{
    int result;

    result = 0;
    data->line_count = file_line_counter(fd, data);
    close(fd);
    data->fd = open_rt_file(data->filename);
    //printf("Número de linhas válidas: %d\n", data->line_count);
    if (data->line_count < 1 || data->line_count < 6)
        return (0 * printf("O arquivo está vazio ou sem linhas válidas\n") + 1);
    while (1)
    {
        data->line = get_next_line(data->fd);
        if (data->line)
        {
            result += process_line(data->line, data);
            if (data->line)
                free(data->line);
        }
        else
            break ;
        if (result == data->line_count)
            return (0);
    }
    return (1);
}

int parse_rt_file(char *filename, t_data *data)
{
    int result;

    data->filename = filename;
    data->fd = open_rt_file(data->filename);
    if (data->fd < 0)
        return (1);
    result = read_and_process_lines(data->fd, data);
    printf("Resultado da leitura do ficheiro: %d\n", result);
    close(data->fd);
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
