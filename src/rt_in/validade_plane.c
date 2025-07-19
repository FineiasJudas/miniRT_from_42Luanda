/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validade_plane.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:39:00 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 15:48:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool parse_plane_components(char *input, char **pos, char **normal, char **rgb)
{
    *pos = take_vector(input);
    if (!*pos) return false;

    char *next = trim(input + ft_strlen(*pos));
    *normal = take_vector(next);
    if (!*normal) return false;

    *rgb = trim(next + ft_strlen(*normal));
    return (*rgb != NULL);
}

char *build_plane_output(char *pos, char *normal, char *rgb)
{
    char *out = ft_strdup("pl ");
    if (!out) return NULL;

    out = ft_strjoin_free(out, removeEspacosETabs(pos));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(normal));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(rgb));

    return out;
}

char *validate_plane(char *str, t_data *data)
{
    char *tmp = trim(strstr(str, "pl") + 2);
    char *pos, *normal, *rgb;

    if (!parse_plane_components(tmp, &pos, &normal, &rgb)) return NULL;

    if (is_valid_vector3d(pos) && is_valid_vector3d(normal) && is_valid_rgb(rgb)) {
        char *out = build_plane_output(pos, normal, rgb);
        free(pos); free(normal);
        return out;
    }

    data->invalid_line = 1;
    free(pos); free(normal);
    return NULL;
}
