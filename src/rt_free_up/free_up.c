/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_up.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:10:24 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/14 15:09:35 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_list	*add_to_list(t_list **garbage, void *ptr)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = ptr;
	new->next = *garbage;
	*garbage = new;
	return (*garbage);
}

void	*gc_malloc(t_list **garbage, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (!add_to_list(garbage, ptr))
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}

void	free_all(t_list **garbage)
{
	t_list	*tmp;

	while (*garbage)
	{
		tmp = *garbage;
		if (tmp->content)
			free(tmp->content);
		*garbage = (*garbage)->next;
		free(tmp);
	}
}

int	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	return (1);
}
