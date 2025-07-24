/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:34:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/24 15:29:27 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

size_t	mat_size(char **m)
{
	size_t	i;

	i = 0;
	if (!m)
		return (0);
	while (m[i] != NULL)
		i++;
	return (i);
}

int	open_rt_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	return (fd);
}

char	*ft_strtok(char *str, const char delim)
{
	static char	*buffer = NULL;
	char		*token_start;

	if (str)
		buffer = str;
	if (!buffer || *buffer == '\0')
		return (NULL);
	token_start = buffer;
	while (*buffer && *buffer != delim)
		buffer++;
	if (*buffer)
	{
		*buffer = '\0';
		buffer++;
	}
	return (token_start);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
