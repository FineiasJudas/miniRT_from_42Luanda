/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_sphere.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:52:19 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 15:52:29 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool parse_sphere_components(char *input, char **pos, char **ratio, char **rgb)
{
    *pos = take_vector(input);
    if (!*pos) return false;

    char *next = trim(input + ft_strlen(*pos));

    int i = 0;
    while (next[i] && !isspace(next[i])) i++;

    *ratio = malloc(i + 1);
    if (!*ratio) return false;

    ft_strlcpy(*ratio, next, i + 1);
    *rgb = trim(next + i);
    return (*rgb != NULL);
}

char *build_sphere_output(char *pos, char *ratio, char *rgb)
{
    char *out = ft_strdup("sp ");
    if (!out) return NULL;

    out = ft_strjoin_free(out, removeEspacosETabs(pos));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(ratio));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(rgb));

    return out;
}

char *validate_sphere(char *str, t_data *data)
{
    char *tmp = trim(strstr(str, "sp") + 2);
    char *position, *ratio, *rgb;

    if (!parse_sphere_components(tmp, &position, &ratio, &rgb)) return NULL;

    if (is_valid_vector3d(position) && is_valid_double(ratio) && is_valid_rgb(rgb)) {
        char *out = build_sphere_output(position, ratio, rgb);
        free(position); free(ratio);
        return out;
    }

    data->invalid_line = 1;
    free(position); free(ratio);
    return NULL;
}
