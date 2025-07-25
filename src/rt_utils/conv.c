/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:49:00 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/25 10:23:29 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	count_digits(char *arg)
{
	char	*base;
	char	*out;
	double	number;

	if (!arg)
		return (-1);
	base = (char *)malloc(sizeof(char) * 2);
	if (!base)
		printf("Malloc não funconou!\n");
	ft_strlcpy(base, "1", 2);
	out = ft_strjoin(base, arg);
	number = ft_atoi(out) - ft_atoi(arg);
	free(out);
	free(base);
	return (number);
}

double	float_convert(char *arg)
{
	char	*integer;
	char	*floater;
	double	in;
	double	f;
	double	out;

	if (!arg || ft_strlen(arg) < 1)
		return (-1);
	floater = ft_strchr(arg, '.');
	if (floater)
	{
		*floater = '\0';
		floater++;
		integer = arg;
	}
	else
	{
		integer = arg;
		in = ft_atoi(integer);
		return ((double)in);
	}
	in = ft_atoi(integer);
	f = ft_atoi(floater);
	out = in + (f / count_digits(floater));
	return (out);
}

t_vector	conv_vector(char *str)
{
	char		**components;
	t_vector	v;

	components = ft_split(str, ',');
	if (!components || count_tokens(components) != 3)
		printf("Coordenadas de vetor inválidas");
	v.x = my_strtod(components[0], NULL);
	v.y = my_strtod(components[1], NULL);
	v.z = my_strtod(components[2], NULL);
	free_tokens(components);
	return (v);
}

t_color	conv_color(char *str)
{
	char	**components;
	t_color	c;

	components = ft_split(str, ',');
	if (!components || count_tokens(components) != 3)
		printf("Coordenadas de vetor inválidas");
	c.r = ft_atoi(components[0]);
	c.g = ft_atoi(components[1]);
	c.b = ft_atoi(components[2]);
	free_tokens(components);
	return (c);
}

void	add_to_scene_list(t_list **list, void *element)
{
	t_list	*node;

	if (!list || !element)
		return ;
	node = ft_lstnew(element);
	if (!node)
	{
		printf("Erro ao alocar memória para objeto\n");
		exit(1);
	}
	ft_lstadd_back(list, node);
}
