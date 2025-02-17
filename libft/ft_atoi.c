/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:27:36 by fjilaias          #+#    #+#             */
/*   Updated: 2024/05/16 17:17:18 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sinal;

	result = 0;
	sinal = 1;
	while (*str && (*str == ' ' || (*str >= 9 && *str <= 13)))
		str ++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sinal = -1;
		str ++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		str ++;
	}
	return (result * sinal);
}
