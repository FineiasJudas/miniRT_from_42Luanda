/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:57:15 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 08:53:34 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// INÍCIO OPERAÇÕES COM VECTORES

t_vector vec_sub(t_vector a, t_vector b)
{
    return (t_vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

float vec_dot(t_vector a, t_vector b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_vector vec_normalize(t_vector v)
{
    float len = sqrtf(vec_dot(v, v));
    if (len == 0)
        return (t_vector){0, 0, 0}; // evitar divisão por 0
    return (t_vector){v.x / len, v.y / len, v.z / len};
}

t_vector vec_scale(t_vector v, float s)
{
    return (t_vector){v.x * s, v.y * s, v.z * s};
}

t_vector vec_add(t_vector a, t_vector b)
{
    return (t_vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

// FIM OPERACOES COM VECTORES

t_color scale_color(t_color color, float factor)
{
    return (t_color){fmin(color.r * factor, 255),
                     fmin(color.g * factor, 255),
                     fmin(color.b * factor, 255)};
}