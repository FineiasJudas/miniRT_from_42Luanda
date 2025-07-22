/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:22:14 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/16 15:28:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	*ler_e_salvar(int fd, char *frase)
{
	char	*buffer;
	int		bytes;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes = 1;
	while (!m_strchr(frase, '\n') && bytes != 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes] = '\0';
		frase = m_strjoin(frase, buffer);
	}
	free(buffer);
	return (frase);
}

static char	*pegar_linha(char *frase)
{
	char	*salvo;
	int		i;

	i = 0;
	if (!frase[i])
		return (NULL);
	while (frase[i] && frase[i] != '\n')
		i++;
	salvo = (char *)malloc(sizeof(char) * (i + 2));
	if (!salvo)
		return (NULL);
	i = 0;
	while (frase[i] && frase[i] != '\n')
	{
		salvo[i] = frase[i];
		i++;
	}
	if (frase[i] == '\n')
	{
		salvo[i] = frase[i];
		i++;
	}
	salvo[i] = '\0';
	return (salvo);
}

static char	*salvar_resto(char *frase)
{
	char	*salvo;
	int		i;
	int		j;

	i = 0;
	while (frase[i] && frase[i] != '\n')
		i++;
	if (!frase[i])
	{
		free(frase);
		return (NULL);
	}
	salvo = (char *)malloc(sizeof(char) * (m_strlen(frase) - i + 1));
	if (!salvo)
		return (NULL);
	i++;
	j = 0;
	while (frase[i])
		salvo[j++] = frase[i++];
	salvo[j] = '\0';
	free(frase);
	return (salvo);
}

char	*get_next_line(int fd)
{
	char		*linha;
	static char	*resto;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	resto = ler_e_salvar(fd, resto);
	if (!resto)
		return (NULL);
	linha = pegar_linha(resto);
	resto = salvar_resto(resto);
	return (linha);
}

t_color	scale_color(t_color color, float factor)
{
	return ((t_color){fmin(color.r * factor, 255), fmin(color.g * factor, 255),
		fmin(color.b * factor, 255)});
}
