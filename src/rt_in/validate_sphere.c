/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_sphere.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:52:19 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/24 15:37:04 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i] != NULL)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

bool	is_valid_fov(const char *str)
{
	char	*endptr;
	double	fov;

	if (!is_valid_double(str))
		return (false);
	fov = my_strtod(str, &endptr);
	if (fov < 0.0 || fov > 180.0)
		return (false);
	return (true);
}

bool	parse_sphere_components(char *input, char **pos, char **ratio,
		char **rgb)
{
	char	*next;
	int		i;

	*pos = take_vector(input);
	if (!*pos)
		return (false);
	next = trim(input + ft_strlen(*pos));
	i = 0;
	while (next[i] && !ft_isspace(next[i]))
		i++;
	*ratio = malloc(i + 1);
	if (!*ratio)
		return (false);
	ft_strlcpy(*ratio, next, i + 1);
	*rgb = trim(next + i);
	return (*rgb != NULL);
}

char	*build_sphere_output(char *pos, char *ratio, char *rgb)
{
	char	*out;

	out = ft_strdup("sp ");
	if (!out)
		return (NULL);
	out = ft_strjoin_free(out, remove_spaces_tabs(pos));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(ratio));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(rgb));
	return (out);
}

char	*validate_sphere(char *str, t_data *data)
{
	char	*tmp;
	char	*out;
	char	*position;
	char	*ratio;
	char	*rgb;

	tmp = trim(ft_strstr(str, "sp") + 2);
	if (!parse_sphere_components(tmp, &position, &ratio, &rgb))
		return (NULL);
	if (is_valid_vector3d(position) && is_valid_positive_double(ratio)
		&& is_valid_rgb(rgb))
	{
		out = build_sphere_output(position, ratio, rgb);
		free(position);
		free(ratio);
		return (out);
	}
	data->invalid_line = 1;
	free(position);
	free(ratio);
	printf("Dado inválido na esfera\n");
	return (NULL);
}
