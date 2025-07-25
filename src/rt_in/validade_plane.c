/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validade_plane.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:39:00 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/24 15:26:32 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;

	if (!s1 || !s2)
		return (NULL);
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	*take_vector(char *input)
{
	int		i_c[2];
	char	*vector;

	i_c[0] = 0;
	i_c[1] = 0;
	while (input[i_c[0]])
	{
		if (i_c[1] == 2 && ft_isalnum(input[i_c[0]]))
		{
			while (input[i_c[0]] && (ft_isalnum(input[i_c[0]])
					|| input[i_c[0]] == '_' || input[i_c[0]] == '.'))
				i_c[0]++;
			break ;
		}
		if (input[i_c[0]] == ',')
			i_c[1]++;
		i_c[0]++;
	}
	if (i_c[1] != 2)
		return (NULL);
	vector = malloc(sizeof(char) * (i_c[0] + 1));
	if (!vector)
		return (NULL);
	ft_strlcpy(vector, input, i_c[0] + 1);
	return (vector[i_c[0]] = '\0', vector);
}

bool	parse_plane_components(char *input, char **pos, char **normal,
		char **rgb)
{
	char	*next;

	*pos = take_vector(input);
	if (!*pos)
		return (false);
	next = trim(input + ft_strlen(*pos));
	*normal = take_vector(next);
	if (!*normal)
		return (false);
	*rgb = trim(next + ft_strlen(*normal));
	return (*rgb != NULL);
}

char	*build_plane_output(char *pos, char *normal, char *rgb)
{
	char	*out;

	out = ft_strdup("pl ");
	if (!out)
		return (NULL);
	out = ft_strjoin_free(out, remove_spaces_tabs(pos));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(normal));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(rgb));
	return (out);
}

char	*validate_plane(char *str, t_data *data)
{
	char	*tmp;
	char	*out;
	char	*pos;
	char	*normal;
	char	*rgb;

	tmp = trim(ft_strstr(str, "pl") + 2);
	if (!parse_plane_components(tmp, &pos, &normal, &rgb))
		return (NULL);
	if (is_valid_vector3d(pos) && is_valid_vector3d(normal)
		&& is_valid_rgb(rgb))
	{
		out = build_plane_output(pos, normal, rgb);
		free(pos);
		free(normal);
		return (out);
	}
	data->invalid_line = 1;
	free(pos);
	free(normal);
	printf("Dado inválido em plano\n");
	return (NULL);
}
