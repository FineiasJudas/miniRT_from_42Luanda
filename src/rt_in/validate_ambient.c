/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_ambient.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 08:43:07 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 13:28:31 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char *trim(char *s)
{
    while (isspace(*s))
        s++;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace(*end))
        *end-- = '\0';
    return s;
}

// Função para validar a parte double
bool is_valid_double(const char* str)
{
    char* endptr;
    strtod(str, &endptr);
    
    // Verifica se houve conversão completa e formato válido
    if (endptr == str || *endptr != '\0') {
        return false;  // Não converteu nada ou não consumiu toda a string
    }
    return true;
}

bool is_valid_rgb(const char* str)
{
    if (!str) return false;
    
    // Conta vírgulas
    int commas = 0;
    for (const char* p = str; *p; p++) {
        if (*p == ',') commas++;
    }
    if (commas != 2) return false;
    // Verifica padrões inválidos
    size_t len = ft_strlen(str);
    if (str[0] == ',' || str[len-1] == ',') return false;
    if (strstr(str, ",,")) return false;
    
    // Processamento dos tokens
    char* copy = ft_strdup(str);
    int count = 0;
    bool valid = true;
    char* token = ft_strtok(copy, ',');
    
    while (token) {
        count++;
        char* cleaned = trim(token);
        
        if (!*cleaned) { // Token vazio após trim
            valid = false;
            break;
        }
        
        char* end;
        long num = strtol(cleaned, &end, 10);
        if (*end || num < 0 || num > 255) {
            valid = false;
            break;
        }
        
        token = ft_strtok(NULL, ',');
    }
    
    free(copy);
    return (valid && count == 3);
}

char* removeEspacosETabs(char *str)
{
    char *src = str, *dst = str;
    while (*src != '\0') {
        if (*src != ' ' && *src != '\t') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

bool parse_ambient_components(char *input, char **brightness, char **rgb)
{
    int i = 0;
    while (isalnum(input[i]) || input[i] == '_' || input[i] == '.' || input[i] == ',')
        i++;

    *brightness = malloc(i + 1);
    if (!*brightness) return false;

    ft_strlcpy(*brightness, input, i + 1);
    *rgb = trim(input + i);

    return (*rgb != NULL);
}

char *build_ambient_output(char *brightness, char *rgb)
{
    char *out = ft_strdup("A ");
    if (!out) return NULL;

    out = ft_strjoin_free(out, removeEspacosETabs(brightness));
    out = ft_strjoin_free(out, " ");
    out = ft_strjoin_free(out, removeEspacosETabs(rgb));
    
    return out;
}

char *validate_ambient(char *str, t_data *data)
{
    char *tmp = trim(strchr(str, 'A') + 1);
    char *brightness, *rgb;

    if (!parse_ambient_components(tmp, &brightness, &rgb)) return NULL;

    if (is_valid_double(brightness) && is_valid_rgb(rgb)) {
        char *out = build_ambient_output(brightness, rgb);
        free(brightness); return out;
    }

    data->invalid_line = 1;
    free(brightness);
    return NULL;
}
