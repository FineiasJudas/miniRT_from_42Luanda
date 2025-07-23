/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:07:00 by lantonio          #+#    #+#             */
/*   Updated: 2025/07/22 15:00:51 by fjilaias         ###   ########.fr       */
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

void	free_list(t_list **list)
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

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*h;
	const char	*n;

	if (!*needle)
		return ((char *)haystack);
	while (*haystack)
	{
		h = haystack;
		n = needle;
		while (*h && *n && *h == *n)
		{
			h++;
			n++;
		}
		if (!*n)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}

long	ft_strtol(const char *nptr, char **endptr, int base)
{
	long		result;
	const char	*p = nptr;

	result = 0;
	if (base != 10)
	{
		if (endptr)
			*endptr = (char *)nptr;
		return (0);
	}
	while (*p >= '0' && *p <= '9')
	{
		result = result * 10 + (*p - '0');
		p++;
	}
	if (endptr)
		*endptr = (char *)p;
	return (result);
}
