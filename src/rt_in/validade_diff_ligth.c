/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validade_diff_ligth.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:27:58 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/31 11:20:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	is_valid_brightness(const char *str)
{
	char	*endptr;
	double	brightness;

	if (!is_valid_positive_double(str))
		return (false);
	brightness = my_strtod(str, &endptr);
	if (brightness < 0.0 || brightness > 1.0)
		return (printf(
				"Error\nA intensidade da luz é de 0.0 à 1.0\n"), false);
	return (true);
}

char	*extract_brightness_component(char *str)
{
	int		i;
	char	*brightness;

	i = 0;
	while (str[i] && !ft_isspace(str[i]))
		i++;
	brightness = malloc(sizeof(char) * (i + 1));
	if (!brightness)
		return (NULL);
	ft_strlcpy(brightness, str, i + 1);
	brightness[i] = '\0';
	return (brightness);
}

bool	validate_light_components(char *pos, char *bri, char *rgb, t_data *data)
{
	if (is_valid_vector3d(pos) && is_valid_brightness(bri) && is_valid_rgb(rgb))
		return (true);
	data->invalid_line = 1;
	return (false);
}

char	*build_light_output(char *pos, char *bri, char *rgb)
{
	char	*out;

	out = ft_strdup("L ");
	if (!out)
		return (NULL);
	out = ft_strjoin_free(out, remove_spaces_tabs(pos));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(bri));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(rgb));
	return (out);
}

char	*validate_light(char *str, t_data *data)
{
	char	*tmp[2];
	char	*position_p;
	char	*next;
	char	*brightness_p;
	char	*rgb_p;

	tmp[0] = trim(ft_strchr(str, 'L') + 1);
	position_p = take_vector(tmp[0]);
	if (!position_p)
		return (NULL);
	next = trim(tmp[0] + ft_strlen(position_p));
	brightness_p = extract_brightness_component(next);
	if (!brightness_p)
		return (free(position_p), NULL);
	rgb_p = trim(next + ft_strlen(brightness_p));
	if (validate_light_components(position_p, brightness_p, rgb_p, data))
	{
		tmp[1] = build_light_output(position_p, brightness_p, rgb_p);
		free(position_p);
		free(brightness_p);
		return (tmp[1]);
	}
	printf("Dado inválido em luz difusa\n");
	return (free(position_p), free(brightness_p), NULL);
}
