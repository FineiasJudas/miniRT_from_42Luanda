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

// Função para validar um vetor 3D (x,y,z)
bool is_valid_vector3d(const char* str)
{
    if (!str)
        return false;

    // Conta vírgulas
    int commas = 0;
    for (const char* p = str; *p; p++) {
        if (*p == ',') commas++;
    }
    if (commas != 2)
        return false;

    // Verifica padrões inválidos
    size_t len = strlen(str);
    if (str[0] == ',' || str[len-1] == ',' || strstr(str, ",,"))
        return false;

    // Processa os tokens
    char* copy = strdup(str);
    if (!copy)
        return false;

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
        
        if (!is_valid_double(cleaned)) {
            valid = false;
            break;
        }
        
        token = strtok(NULL, ",");
    }

    free(copy);
    return (valid && count == 3);
}

// Função para validar o FOV (deve ser um double entre 0 e 180)
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

// Função para validar a linha da câmera
char *validate_camera(char *str)
{
    char *tmp;
    char *origin_p = NULL;
    char *direction_p = NULL;
    char *fov_p;
    char *out = NULL;
    int i = 0;
    int commas = 0;

    // Verifica se str é válido e começa com 'C'
    if (!str || !strchr(str, 'C')) {
        free(str);
        return (NULL);
    }

    // Trim da parte após o 'C'
    tmp = trim(strchr(str, 'C') + 1);
    
    // Calcula o tamanho da parte do vetor de origem
    while (tmp[i])
    {
        if (commas == 2 && ft_isalnum(tmp[i]))
        {
            while (tmp[i] && (ft_isalnum(tmp[i]) || tmp[i] == '_'))
                i ++;
            break ;
        }
        if (tmp[i] == ',')
            commas ++;
        i ++;
    }
    if (commas != 2) {
        free(str);
        return (NULL);
    }
    
    // Aloca origin_p
    origin_p = malloc(sizeof(char) * (i + 1));
    if (!origin_p) {
        free(str);
        return (NULL);
    }
    
    ft_strlcpy(origin_p, tmp, i + 1);
    origin_p[i] = '\0';

    // Trim da parte do vetor de direção
    char *next = trim(tmp + i);
    i = 0;
    commas = 0;
    while (next[i])
    {
        if (commas == 2 && ft_isalnum(next[i]))
        {
            while (next[i] && (ft_isalnum(next[i]) || next[i] == '_'))
                i ++;
            break ;
        }
        if (next[i] == ',')
            commas ++;
        i ++;
    }
    if (commas != 2) {
        free(origin_p);
        free(str);
        return (NULL);
    }
    
    // Aloca direction_p
    direction_p = malloc(sizeof(char) * (i + 1));
    if (!direction_p) {
        free(origin_p);
        free(str);
        return (NULL);
    }
    
    ft_strlcpy(direction_p, next, i + 1);
    direction_p[i] = '\0';
    
    // Trim da parte do FOV
    fov_p = trim(next + i);
    
    // Valida os componentes
    if (is_valid_vector3d(origin_p) && is_valid_vector3d(direction_p) && is_valid_fov(fov_p))
    {
        // Constrói a string de saída
        out = ft_strdup("C ");
        if (!out) {
            free(origin_p);
            free(direction_p);
            free(str);
            return (NULL);
        }
        
        char *temp = ft_strjoin(out, removeEspacosETabs(origin_p));
        if (!temp) {
            free(origin_p);
            free(direction_p);
            free(str);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(origin_p);
            free(direction_p);
            free(str);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(direction_p));
        if (!temp) {
            free(origin_p);
            free(direction_p);
            free(str);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(origin_p);
            free(direction_p);
            free(str);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(fov_p));
        if (!temp) {
            free(origin_p);
            free(direction_p);
            free(str);
            free(out);
            return (NULL);
        }
        out = temp;
        
        printf("Dados certos!\n\n");
        printf("FINAL - %s\n", out);
    }
    else
    {
        printf("Dados errados\n");
        free(origin_p);
        free(direction_p);
        free(str);
        return (NULL);
    }
    
    // Libera memória
    free(origin_p);
    free(direction_p);
    free(str);
    
    return (out);
}