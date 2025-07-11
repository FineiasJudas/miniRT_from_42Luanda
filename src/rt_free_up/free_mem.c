/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:07:00 by lantonio          #+#    #+#             */
/*   Updated: 2025/07/10 11:37:44 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_mem(t_list **list)
{
	t_list	*cur;

	while (*list)
	{
		cur = (*list)->next;
		if ((*list)->content)
		{
			free((*list)->content);
			(*list)->content = NULL;
		}
		free(*list);
		*list = NULL;
		*list = cur;
	}
}

void free_list(t_list **list)
{
	t_list	*cur;

	while (*list)
	{
		cur = (*list)->next;
		free(*list);
		*list = NULL;
		*list = cur;
	}
}
