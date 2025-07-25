/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:53:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/25 09:33:54 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_camera(char **tokens, t_data *scene)
{
	if (!tokens[1] || !tokens[2] || !tokens[3])
		return (0);
	scene->camera = (t_camera *)malloc(sizeof(t_camera));
	if (!scene->camera)
		return (0);
	collect_mem(scene->camera);
	//scene->camera->dir = vec_normalize(conv_vector(tokens[1]));
	if (scene->camera->dir.x == 0 && scene->camera->dir.y == 0
		&& scene->camera->dir.z == 0)
	{
		printf("Direção da câmera inválida\n");
		return (0);
	}
	if (scene->camera->dir.z < 0)
	{
		scene->camera->dir.x *= -1;
		scene->camera->dir.y *= -1;
		scene->camera->dir.z *= -1;
	}
	//scene->camera->dir = conv_vector(tokens[1]);
	scene->camera->dir = vec_normalize(conv_vector(tokens[2]));
	scene->camera->origin = conv_vector(tokens[1]);
	scene->camera->fov = ft_atoi(tokens[3]);
	return (1);
}

int	identify_and_process(char **tokens, t_data *data)
{
	int	result;

	if (!tokens || !tokens[0])
		return (1);
	result = 0;
	if (ft_strncmp(tokens[0], "A", 1) == 0)
		result = parse_ambient(tokens, data);
	else if (ft_strncmp(tokens[0], "C", 1) == 0)
		result = parse_camera(tokens, data);
	else if (ft_strncmp(tokens[0], "L", 1) == 0)
		result = parse_light(tokens, data);
	else if (ft_strncmp(tokens[0], "sp", 2) == 0)
		result = parse_sphere(tokens, data);
	else if (ft_strncmp(tokens[0], "pl", 2) == 0)
		result = parse_plane(tokens, data);
	else if (ft_strncmp(tokens[0], "cy", 2) == 0)
		result = parse_cylinder(tokens, data);
	return (result);
}

int	process_line(char *line, t_data *data)
{
	char	**tokens;
	int		result;

	result = 0;
	if (!line || ft_strlen(line) < 1)
		return (1);
	tokens = ft_split(line, ' ');
	if (!tokens)
		return (1);
	result = identify_and_process(tokens, data);
	free_tokens(tokens);
	if (line)
		free(line);
	return (result);
}

char	*identify_type(char *line, t_data *data)
{
	char	*result;
	char	**tokens;

	if (!line || ft_strlen(line) < 1)
		return (NULL);
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (NULL);
	result = NULL;
	if (ft_strncmp(tokens[0], "A", 1) == 0)
		result = validate_ambient(line, data);
	else if (ft_strncmp(tokens[0], "C", 1) == 0)
		result = validate_camera(line, data);
	else if (ft_strncmp(tokens[0], "L", 1) == 0)
		result = validate_light(line, data);
	else if (ft_strncmp(tokens[0], "sp", 2) == 0)
		result = validate_sphere(line, data);
	else if (ft_strncmp(tokens[0], "pl", 2) == 0)
		result = validate_plane(line, data);
	else if (ft_strncmp(tokens[0], "cy", 2) == 0)
		result = validate_cylinder(line, data);
	free_matrix(tokens);
	return (result);
}

int	parse_rt_file(char *filename, t_data *data)
{
	int		i;
	int		line_count;
	char	**lines;
	char	*validated;

	line_count = 0;
	data->fd = open_rt_file(filename);
	if (data->fd < 0)
		return (1);
	lines = read_file_into_matrix(data->fd, &line_count, data);
	close(data->fd);
	if (!lines || data->invalid_line || !essential_tokens(lines))
		return (free_matrix(lines), 1);
	i = -1;
	while (++i < line_count)
	{
		validated = identify_type(lines[i], data);
		if (validated)
			process_line(validated, data);
		else
			return (free_data(data), free_matrix(lines), 1);
	}
	free_matrix(lines);
	return (0);
}
