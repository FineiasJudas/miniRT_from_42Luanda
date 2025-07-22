/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_mem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:06:15 by lantonio          #+#    #+#             */
/*   Updated: 2025/07/21 15:43:41 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	free_data(t_data *data)
{
	free_list(&data->sphere_l);
	free_list(&data->cylinder_l);
	free_list(&data->lights_l);
	free_list(&data->plane_l);
	free_mem(get_mem_address());
	return (0);
}

t_list	**get_mem_address(void)
{
	static t_list	*ptr;

	return (&ptr);
}

void	collect_mem(void *content)
{
	ft_lstadd_back(get_mem_address(), ft_lstnew(content));
}

void	*allocate_mem(size_t nmemb, size_t size)
{
	void	*content;

	content = ft_calloc(nmemb, size);
	if (content == NULL)
		exit(1);
	collect_mem(content);
	return (content);
}

int	essential_tokens(char **matriz)
{
	int		i;
	int		alc[3];
	char	**tokens;

	if (!matriz || (mat_size(matriz) < 3))
		return (0);
	i = 0;
	alc[0] = 0;
	alc[1] = 0;
	alc[2] = 0;
	while (matriz[i])
	{
		tokens = ft_split(matriz[i], ' ');
		if (ft_strncmp(tokens[0], "A", 1) == 0)
			alc[0]++;
		else if (ft_strncmp(tokens[0], "L", 1) == 0)
			alc[1]++;
		else if (ft_strncmp(tokens[0], "C", 1) == 0)
			alc[2]++;
		i++;
		free_matrix(tokens);
	}
	if (alc[0] == 1 && alc[1] == 1 && alc[2] == 1)
		return (1);
	return (0);
}
