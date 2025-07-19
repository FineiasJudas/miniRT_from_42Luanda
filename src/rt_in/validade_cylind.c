/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validade_clind.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:49:58 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 15:51:54 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool is_valid_positive_double(const char* str)
{
    if (!is_valid_double(str))
        return false;
    
    char* endptr;
    double value = strtod(str, &endptr);
    if (value <= 0.0)
        return false;
    
    return true;
}

bool parse_cylinder_components(char *input, char **center, char **normal, char **diam, char **height, char **rgb)
{
    *center = take_vector(input);
    if (!*center) return false;

    char *next = trim(input + ft_strlen(*center));
    *normal = take_vector(next);
    if (!*normal) return false;

    next = trim(next + ft_strlen(*normal));
    int i = 0; while (next[i] && !isspace(next[i])) i++;
    *diam = strndup(next, i); if (!*diam) return false;

    next = trim(next + i);
    i = 0; while (next[i] && !isspace(next[i])) i++;
    *height = strndup(next, i); if (!*height) return false;

    *rgb = trim(next + i);
    return (*rgb != NULL);
}

char *build_cylinder_output(char *center, char *normal, char *diam, char *height, char *rgb)
{
    char *out = ft_strdup("cy ");
    if (!out) return NULL;

    out = ft_strjoin_free(out, removeEspacosETabs(center));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(normal));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(diam));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(height));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(rgb));

    return out;
}

char *validate_cylinder(char *str, t_data *data)
{
    char *tmp = trim(strstr(str, "cy") + 2);
    char *center, *normal, *diam, *height, *rgb;

    if (!parse_cylinder_components(tmp, &center, &normal, &diam, &height, &rgb)) return NULL;

    if (is_valid_vector3d(center) && is_valid_vector3d(normal) &&
        is_valid_positive_double(diam) && is_valid_positive_double(height) &&
        is_valid_rgb(rgb)) {
        
        char *out = build_cylinder_output(center, normal, diam, height, rgb);
        free(center); free(normal); free(diam); free(height);
        return out;
    }

    data->invalid_line = 1;
    free(center); free(normal); free(diam); free(height);
    return NULL;
}
