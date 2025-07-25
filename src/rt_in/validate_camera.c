/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_camera.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:40:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/24 15:26:48 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	has_valid_vector_format(const char *str)
{
	int		commas;
	size_t	len;
	int		i;

	commas = 0;
	len = ft_strlen(str);
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			commas++;
		i++;
	}
	if (commas != 2 || str[0] == ',' || str[len - 1] == ',' || ft_strstr(str,
			",,"))
		return (false);
	return (true);
}

bool	is_valid_vector3d(const char *str)
{
	char	*copy;
	int		count;
	char	*token;
	char	*cleaned;

	if (!has_valid_vector_format(str))
		return (false);
	copy = ft_strdup(str);
	if (!copy)
		return (false);
	count = 0;
	token = ft_strtok(copy, ',');
	while (token)
	{
		count++;
		cleaned = trim(token);
		if (!*cleaned || !is_valid_double(cleaned))
			return (free(copy), false);
		token = ft_strtok(NULL, ',');
	}
	free(copy);
	return (count == 3);
}

bool	parse_camera_components(char *input, char **origin, char **direction,
		char **fov)
{
	char	*next;

	*origin = take_vector(input);
	if (!*origin)
		return (false);
	next = trim(input + ft_strlen(*origin));
	*direction = take_vector(next);
	if (!*direction)
		return (false);
	*fov = trim(next + ft_strlen(*direction));
	return (*fov != NULL);
}

char	*build_camera_output(char *origin, char *direction, char *fov)
{
	char	*out;

	out = ft_strdup("C ");
	if (!out)
		return (NULL);
	out = ft_strjoin_free(out, remove_spaces_tabs(origin));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(direction));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(fov));
	return (out);
}

char	*validate_camera(char *str, t_data *data)
{
	char	*tmp;
	char	*out;
	char	*origin;
	char	*direction;
	char	*fov;

	tmp = trim(ft_strchr(str, 'C') + 1);
	if (!parse_camera_components(tmp, &origin, &direction, &fov))
	{
		return (NULL);
	}
	if (is_valid_vector3d(origin) && is_valid_vector3d(direction)
		&& is_valid_fov(fov))
	{
		out = build_camera_output(origin, direction, fov);
		free(origin);
		free(direction);
		return (out);
	}
	data->invalid_line = 1;
	free(origin);
	free(direction);
	printf("Dado inválido em câmera\n");
	return (NULL);
}
