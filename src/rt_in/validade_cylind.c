/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validade_cylind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:49:58 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/31 12:01:37 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	is_valid_double(const char *str)
{
	char	*endptr;

	my_strtod(str, &endptr);
	if (endptr == str || *endptr != '\0')
		return (printf("Error\nValor double inválido\n"), false);
	return (true);
}

bool	is_valid_positive_double(const char *str)
{
	char	*endptr;
	double	value;

	if (!is_valid_double(str))
		return (false);
	value = my_strtod(str, &endptr);
	if (value <= 0.0)
		return (printf("Error\nValor double não positivo\n"), false);
	return (true);
}

bool	parse_cylinder_components(char *input, char **center, char **normal,
		char **d_h_r)
{
	char	*next;
	int		i;

	*center = take_vector(input);
	if (!*center)
		return (false);
	next = trim(input + ft_strlen(*center));
	*normal = take_vector(next);
	if (!*normal)
		return (false);
	next = trim(next + ft_strlen(*normal));
	i = 0;
	while (next[i] && !ft_isspace(next[i]))
		i++;
	d_h_r[0] = ft_strndup(next, i);
	if (!d_h_r[0])
		return (false);
	next = trim(next + i);
	i = 0;
	while (next[i] && !ft_isspace(next[i]))
		i++;
	d_h_r[1] = ft_strndup(next, i);
	if (!d_h_r[1])
		return (false);
	return (d_h_r[2] = trim(next + i), d_h_r[2] != NULL);
}

char	*build_cylinder_output(char *center, char *normal, char **d_h_r)
{
	char	*out;

	out = ft_strdup("cy ");
	if (!out)
		return (NULL);
	out = ft_strjoin_free(out, remove_spaces_tabs(center));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(normal));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(d_h_r[0]));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(d_h_r[1]));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(d_h_r[2]));
	return (out);
}

char	*validate_cylinder(char *str, t_data *data)
{
	char	*tmp;
	char	*out;
	char	*center;
	char	*normal;
	char	**d_h_r;

	tmp = trim(ft_strstr(str, "cy") + 2);
	d_h_r = (char **)malloc(sizeof(char *) * 3);
	if (!parse_cylinder_components(tmp, &center, &normal, d_h_r))
		return (NULL);
	if (is_valid_vector3d(center) && is_valid_vector3d(normal)
		&& is_valid_positive_double(d_h_r[0])
		&& is_valid_positive_double(d_h_r[1]) && is_valid_rgb(d_h_r[2]))
	{
		out = build_cylinder_output(center, normal, d_h_r);
		free(center);
		free(normal);
		free(d_h_r[0]);
		free(d_h_r[1]);
		return (free(d_h_r), out);
	}
	data->invalid_line = 1;
	free(center);
	printf("Dado inválido no cilindro\n");
	return (free(normal), free(d_h_r[0]), free(d_h_r[1]), free(d_h_r), NULL);
}
