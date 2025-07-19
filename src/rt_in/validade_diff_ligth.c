/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validade_diff_ligth.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:27:58 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 15:37:43 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char *ft_strjoin_free(char *s1, const char *s2)
{
    char *joined;
    
    if (!s1 || !s2)
        return NULL;

    joined = ft_strjoin(s1, s2);
    free(s1);
    return joined;
}

bool is_valid_brightness(const char* str)
{
    if (!is_valid_double(str))
        return false;
    
    char* endptr;
    double brightness = strtod(str, &endptr);
    if (brightness < 0.0 || brightness > 1.0)
        return false;
    
    return true;
}

char *take_vector(char *input)
{
    int i = 0, commas = 0;

    while (input[i])
    {
        if (commas == 2 && ft_isalnum(input[i]))
        {
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_' || input[i] == '.'))
                i ++;
            break ;
        }
        if (input[i] == ',')
            commas ++;
        i ++;
    }

    if (commas != 2)
        return NULL;

    char *vector = malloc(sizeof(char) * (i + 1));
    if (!vector)
        return NULL;

    ft_strlcpy(vector, input, i + 1);
    vector[i] = '\0';

    return vector;
}

char *extract_brightness_component(char *str)
{
    int i = 0;

    while (str[i] && !isspace(str[i]))
        i++;

    char *brightness = malloc(sizeof(char) * (i + 1));
    if (!brightness)
        return NULL;

    ft_strlcpy(brightness, str, i + 1);
    brightness[i] = '\0';

    return brightness;
}

bool validate_light_components(char *pos, char *bri, char *rgb, t_data *data)
{
    if (is_valid_vector3d(pos) &&
        is_valid_brightness(bri) &&
        is_valid_rgb(rgb))
        return true;

    data->invalid_line = 1;
    return false;
}

char *build_light_output(char *pos, char *bri, char *rgb)
{
    char *out = ft_strdup("L ");
    if (!out) return NULL;

    out = ft_strjoin_free(out, removeEspacosETabs(pos));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(bri));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(rgb));

    return out;
}

char *validate_light(char *str, t_data *data)
{
    char *tmp = trim(strchr(str, 'L') + 1);
    char *position_p = take_vector(tmp);
    if (!position_p) return NULL;

    char *next = trim(tmp + ft_strlen(position_p));
    char *brightness_p = extract_brightness_component(next);
    if (!brightness_p) {
        free(position_p);
        return NULL;
    }

    char *rgb_p = trim(next + ft_strlen(brightness_p));

    if (validate_light_components(position_p, brightness_p, rgb_p, data)) {
        char *out = build_light_output(position_p, brightness_p, rgb_p);
        free(position_p);
        free(brightness_p);
        return out;
    }

    free(position_p);
    free(brightness_p);
    return NULL;
}
