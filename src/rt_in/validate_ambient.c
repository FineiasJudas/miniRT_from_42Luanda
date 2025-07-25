/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_ambient.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 08:43:07 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/25 10:01:36 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*trim(char *s)
{
	char	*end;

	while (ft_isspace(*s))
		s++;
	end = s + ft_strlen(s) - 1;
	while (end > s && ft_isspace(*end))
		*end-- = '\0';
	return (s);
}

char	*remove_spaces_tabs(char *str)
{
	char	*src;
	char	*dst;

	src = str;
	dst = str;
	while (*src != '\0')
	{
		if (*src != ' ' && *src != '\t')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
	return (str);
}

bool	parse_ambient_components(char *input, char **brightness, char **rgb)
{
	int	i;

	i = 0;
	while (isalnum(input[i]) || input[i] == '_' || input[i] == '.'
		|| input[i] == ',')
		i++;
	*brightness = malloc(i + 1);
	if (!*brightness)
		return (false);
	ft_strlcpy(*brightness, input, i + 1);
	*rgb = trim(input + i);
	return (*rgb != NULL);
}

char	*build_ambient_output(char *brightness, char *rgb)
{
	char	*out;

	out = ft_strdup("A ");
	if (!out)
		return (NULL);
	out = ft_strjoin_free(out, remove_spaces_tabs(brightness));
	out = ft_strjoin_free(out, " ");
	out = ft_strjoin_free(out, remove_spaces_tabs(rgb));
	return (out);
}

char	*validate_ambient(const char *str, t_data *data)
{
	char	*tmp;
	char	*out;
	char	*brightness;
	char	*rgb;

	tmp = trim(ft_strchr(str, 'A') + 1);
	if (!parse_ambient_components(tmp, &brightness, &rgb))
		return (NULL);
	if (is_valid_brightness(brightness) && is_valid_rgb(rgb))
	{
		out = build_ambient_output(brightness, rgb);
		free(brightness);
		return (out);
	}
	data->invalid_line = 1;
	free(brightness);
	printf("Dado inválido em luz ambiente\n");
	return (NULL);
}
