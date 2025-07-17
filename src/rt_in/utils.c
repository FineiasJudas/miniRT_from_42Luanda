/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:34:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/16 16:09:33 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_delim(char c, const char *delim)
{
	while (*delim)
	{
		if (c == *delim)
			return (1);
		delim++;
	}
	return (0);
}

int	open_rt_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	return (fd);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*next_token;
	char		*token_start;
	char		*token_end;

	next_token = NULL;
	if (str)
		next_token = str;
	if (!next_token || !*next_token)
		return (NULL);
	token_start = next_token;
	token_end = token_start;
	while (*token_end && !is_delim(*token_end, delim))
		token_end++;
	if (*token_end)
		*token_end++ = '\0';
	next_token = token_end;
	return (token_start);
}

int	is_first_word_one_of(char *line)
{
	char	*first_word;

	if (!line || ft_strlen(line) < 1)
		return (0);
	first_word = ft_strtok(line, " ");
	if (!first_word)
		return (0);
	if (ft_strncmp(first_word, "A", 1) == 0 || ft_strncmp(first_word, "C",
			1) == 0 || ft_strncmp(first_word, "L", 1) == 0
		|| ft_strncmp(first_word, "sp", 2) == 0 || ft_strncmp(first_word, "pl",
			2) == 0 || ft_strncmp(first_word, "cy", 2) == 0)
		return (1);
	return (0);
}
