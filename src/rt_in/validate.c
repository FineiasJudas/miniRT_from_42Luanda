/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:46:36 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/24 15:27:04 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	*first_token(const char *s)
{
	int		len;
	char	*tok;

	len = 0;
	while (s[len] && (s[len] == ' ' || s[len] == '\t'))
		++s;
	while (s[len] && s[len] != ' ' && s[len] != '\t' && s[len] != '\n')
		++len;
	tok = ft_substr(s, 0, len);
	return (tok);
}

static void	valid_identifier(const char *tok, t_data *data)
{
	if (ft_strncmp(tok, "sp", 2) == 0 || ft_strncmp(tok, "cy", 2) == 0
		|| ft_strncmp(tok, "pl", 2) == 0 || ft_strncmp(tok, "A", 1) == 0
		|| ft_strncmp(tok, "L", 1) == 0 || ft_strncmp(tok, "C", 1) == 0)
		data->invalid_line = 0;
	else
	{
		data->invalid_line = 1;
		printf("Linha inválida\n");
	}
}

static char	**append_line(char **matrix, int *count, char *line)
{
	char	**tmp;

	tmp = ft_realloc(matrix, sizeof(char *) * mat_size(matrix),
			sizeof(char *) * (*count + 2));
	if (!tmp)
		return (NULL);
	matrix = tmp;
	matrix[*count] = line;
	(*count)++;
	matrix[*count] = NULL;
	return (matrix);
}

static void	free_partial(char **matrix, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

char	**read_file_into_matrix(int fd, int *out_count, t_data *data)
{
	int		count;
	char	*tok;

	count = 0;
	data->line = get_next_line(fd);
	while (data->line)
	{
		if (!is_blank_line(data->line))
		{
			tok = first_token(data->line);
			valid_identifier(tok, data);
			free(tok);
			data->matrix = append_line(data->matrix, &count, data->line);
			if (!data->matrix || data->invalid_line)
			{
				free_partial(data->matrix, count);
				return (NULL);
			}
		}
		else
			free(data->line);
		data->line = get_next_line(fd);
	}
	*out_count = count;
	return (data->matrix);
}
