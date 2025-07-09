/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_up.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:10:24 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/09 11:50:47 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_list *add_to_list(t_list **garbage, void *ptr)
{
	t_list *new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = ptr;
	new->next = *garbage;
	*garbage = new;
	return (*garbage);
}

void *gc_malloc(t_list **garbage, size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (!add_to_list(garbage, ptr))
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}

void free_all(t_list **garbage)
{
	t_list *tmp;

	while (*garbage)
	{
		tmp = *garbage;
		if (tmp->content)
			free(tmp->content);
		*garbage = (*garbage)->next;
		free(tmp);
	}
}


/*void *my_malloc(size_t size)
{
    void *data = malloc(size);
    if (data == NULL)
    {
        t_list *new_node = (t_list *)malloc(sizeof(t_list));
        if (new_node) {
            new_node->content = data;
            new_node->next = head;
            head = new_node;
        } else
        {
            free(data);
            return NULL;
        }
    }
    return data;
}

void my_free(void *ptr)
{
    if (!ptr) return;

    t_list **current = &head;
    while (*current) {
        if ((*current)->content == ptr) {
            t_list *to_free = *current;
            *current = (*current)->next;
            free(to_free->content);
            free(to_free);
            return;
        }
        current = &(*current)->next;
    }
    // If we reach here, the pointer was not found in the list
}

void free_all() {
    t_list *current = head;
    while (current) {
        t_list *next = current->next;
        free(current->content);
        free(current);
        current = next;
    }
    head = NULL;
}*/