/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:07:45 by fjilaias          #+#    #+#             */
/*   Updated: 2024/05/16 18:17:34 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char	*origem;
	unsigned char		*destino;
	size_t				i;

	origem = (const unsigned char *)src;
	destino = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		destino[i] = origem[i];
		i ++;
	}
	return (dest);
}
