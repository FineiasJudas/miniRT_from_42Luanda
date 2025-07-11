/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_mem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:06:15 by lantonio          #+#    #+#             */
/*   Updated: 2025/07/10 11:37:37 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

/*void	collect_env_mem(char **env_table)
{
	int	i;

	if (get_env_address())
		free_mem(get_env_address());
	i = 0;
	ft_lstadd_back(get_env_address(), ft_lstnew((void *)env_table));
	while (env_table[i])
	{
		ft_lstadd_back(get_env_address(), ft_lstnew((void *)env_table[i]));
		i++;
	}
}*/
