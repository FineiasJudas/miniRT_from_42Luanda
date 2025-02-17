/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 11:41:18 by fjilaias          #+#    #+#             */
/*   Updated: 2024/05/16 18:11:28 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	count;
	size_t	sub_len;
	size_t	lc;
	char	*bbig;

	bbig = (char *)big;
	sub_len = ft_strlen(little);
	if (sub_len == 0)
		return (bbig);
	count = 0;
	while (big[count] != '\0' && count < len)
	{
		lc = 0;
		while (big[count + lc] == little[lc] && count + lc < len)
		{
			if (little[lc + 1] == '\0')
				return (&bbig[count]);
			lc++;
		}
		count++;
	}
	return (NULL);
}
