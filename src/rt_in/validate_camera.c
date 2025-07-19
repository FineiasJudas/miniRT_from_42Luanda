/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_camera.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:40:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 15:19:13 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool has_valid_vector_format(const char* str)
{
    int commas = 0;
    size_t len = ft_strlen(str);
    int i = 0;
    while (str[i])
    {
        if (str[i] == ',')
            commas++;
        i++;
    }
    if (commas != 2 || str[0] == ',' || str[len - 1] == ',' || strstr(str, ",,"))
        return false;
    return true;
}

bool is_valid_vector3d(const char* str)
{
    if (!has_valid_vector_format(str))
        return false;
    char* copy = ft_strdup(str);
    if (!copy)
        return false;
    int count = 0;
    bool valid = true;
    char* token = ft_strtok(copy, ',');
    while (token)
    {
        printf("TOKEN - %s\n", token);
        count++;
        char* cleaned = trim(token);
        if (!*cleaned || !is_valid_double(cleaned))
        {
            valid = false;
            break;
        }
        token = ft_strtok(NULL, ',');
    }
    free(copy);
    return (valid && count == 3);
}


bool is_valid_fov(const char* str)
{
    if (!is_valid_double(str))
        return false;
    
    char* endptr;
    double fov = strtod(str, &endptr);
    if (fov < 0.0 || fov > 180.0)
        return false;
    
    return true;
}

bool parse_camera_components(char *input, char **origin, char **direction, char **fov)
{
    *origin = take_vector(input);
    if (!*origin) return false;

    char *next = trim(input + ft_strlen(*origin));
    *direction = take_vector(next);
    if (!*direction) return false;

    *fov = trim(next + ft_strlen(*direction));
    return (*fov != NULL);
}

char *build_camera_output(char *origin, char *direction, char *fov)
{
    char *out = ft_strdup("C ");
    if (!out) return NULL;

    out = ft_strjoin_free(out, removeEspacosETabs(origin));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(direction));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(fov));

    return out;
}

char *validate_camera(char *str, t_data *data)
{
    char *tmp = trim(strchr(str, 'C') + 1);
    char *origin , *direction , *fov ;
    if (!parse_camera_components(tmp, &origin, &direction, &fov)) {
        return NULL;
    }

    if (is_valid_vector3d(origin) && is_valid_vector3d(direction) && is_valid_fov(fov)) {
        char *out = build_camera_output(origin, direction, fov);
        free(origin); free(direction);
        return out;
    }

    data->invalid_line = 1;
    free(origin); free(direction);
    return NULL;
}
