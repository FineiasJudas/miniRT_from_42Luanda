/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:12:57 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/31 11:10:50 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	is_valid_rgb_component(const char *str)
{
	char	*end;
	long	num;

	num = ft_strtol(str, &end, 10);
	return (*end == '\0' && num >= 0 && num <= 255);
}

bool	validate_rgb_values(const char *str)
{
	char	*copy;
	char	*token;
	int		count;
	bool	valid;
	char	*cleaned;

	copy = ft_strdup(str);
	token = ft_strtok(copy, ',');
	count = 0;
	valid = true;
	while (token && count < 3)
	{
		cleaned = trim(token);
		if (!is_valid_rgb_component(cleaned))
		{
			valid = false;
			break ;
		}
		token = ft_strtok(NULL, ',');
		count++;
	}
	free(copy);
	return (valid && count == 3);
}

bool	valid_structure(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (str[0] == ',' || str[len - 1] == ',')
		return (false);
	if (ft_strstr(str, ",,"))
		return (false);
	return (true);
}

bool	valid_commas(const char *str)
{
	int	commas;
	int	i;

	i = 0;
	commas = 0;
	while (str[i])
	{
		if (str[i] == ',')
			commas++;
		i ++;
	}
	return (commas == 2);
}

bool	is_valid_rgb(const char *str)
{
	if (!str || !valid_commas(str) || !valid_structure(str))
		return (printf("Error\nRGB inválido\n"), false);
	if (validate_rgb_values(str))
		return (true);
	return (printf("Error\nRGB inválido\n"), false);
}
