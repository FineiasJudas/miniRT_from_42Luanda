/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:23:03 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/22 13:07:55 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static const char	*skip_spaces(const char *p)
{
	while (ft_isspace((unsigned char)*p))
		p++;
	return (p);
}

static const char	*parse_sign(const char *p, int *sign)
{
	*sign = 1;
	if (*p == '-')
	{
		*sign = -1;
		p++;
	}
	else if (*p == '+')
	{
		p++;
	}
	return (p);
}

static const char	*parse_integer_part(const char *p, double *value,
		int *has_digits)
{
	*value = 0.0;
	*has_digits = 0;
	while (ft_isdigit((unsigned char)*p))
	{
		*value = *value * 10.0 + (*p - '0');
		p++;
		*has_digits = 1;
	}
	return (p);
}

static const char	*parse_fraction_part(const char *p, double *value,
		int *has_digits)
{
	double	divisor;

	if (*p == '.')
	{
		p++;
		divisor = 10.0;
		while (ft_isdigit((unsigned char)*p))
		{
			*value += (*p - '0') / divisor;
			divisor *= 10.0;
			p++;
			*has_digits = 1;
		}
	}
	return (p);
}

double	my_strtod(const char *__restrict__ nptr, char **__restrict__ endptr)
{
	const char	*p = nptr;
	int			sign;
	double		value;
	int			has_digits;

	p = skip_spaces(p);
	p = parse_sign(p, &sign);
	p = parse_integer_part(p, &value, &has_digits);
	p = parse_fraction_part(p, &value, &has_digits);
	if (endptr != NULL)
	{
		if (has_digits)
			*endptr = (char *)p;
		else
			*endptr = (char *)nptr;
	}
	return (sign * value);
}
