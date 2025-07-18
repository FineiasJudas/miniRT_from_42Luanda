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
    size_t len = strlen(str);
    if (str[0] == ',' || str[len-1] == ',') return false;
    if (strstr(str, ",,")) return false;
    
    // Processamento dos tokens
    char* copy = strdup(str);
    int count = 0;
    bool valid = true;
    char* token = strtok(copy, ",");
    
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
        
        token = strtok(NULL, ",");
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

char *validate_ambient(char *str, t_data *data)
{
    int i = 0;
    char *tmp;
    char *double_p = NULL;
    char *rgb_p;
    char *out = NULL;
    tmp = trim(strchr(str, 'A') + 1);
    if (!tmp)
        return NULL;
    while (isalnum(tmp[i]) || tmp[i] == '_' || tmp[i] == '.' || tmp[i] == ',')
        i++;
    double_p = malloc(sizeof(char) * (i + 1));
    if (!double_p)
        return (NULL);
    ft_strlcpy(double_p, tmp, i + 1);
    rgb_p = trim(tmp + strlen(double_p));
    if (is_valid_double(double_p) && is_valid_rgb(rgb_p))
    {
        out = ft_strdup("A ");
        if (!out)
        {
            free(double_p);
            return (NULL);
        }
        char *temp = ft_strjoin(out, removeEspacosETabs(double_p));
        if (!temp)
        {
            free(double_p);
            free(out);
            return (NULL);
        }
        out = temp;
        temp = ft_strjoin(out, " ");
        if (!temp)
        {
            free(double_p);
            free(out);
            return (NULL);
        }
        out = temp;
        temp = ft_strjoin(out, removeEspacosETabs(rgb_p));
        if (!temp)
        {
            free(double_p);
            free(out);
            return (NULL);
        }
        out = temp;
        printf("Dados certos!\n\n");
    }
    else
    {
        data->invalid_line = 1;
        free(double_p);
        return (NULL); // Retorna NULL em caso de erro
    }
    free(double_p);
    // Libera a string de entrada, se necessário
    return (out);
}
