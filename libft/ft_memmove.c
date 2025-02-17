/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 09:44:26 by fjilaias          #+#    #+#             */
/*   Updated: 2024/05/16 18:20:54 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*origem;
	unsigned char		*destino;
	size_t				i;

	i = n;
	origem = (const unsigned char *)src;
	destino = (unsigned char *)dest;
	if (origem < destino && destino < origem + n)
	{
		while (i > 0)
		{
			destino[i - 1] = origem[i - 1];
			i --;
		}
	}
	else
	{
		i = 0;
		while (i < n)
		{
			destino[i] = origem[i];
			i ++;
		}
	}
	return (dest);
}
