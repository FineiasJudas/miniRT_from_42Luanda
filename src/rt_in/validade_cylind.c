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

// Função para validar um double positivo (usado para diâmetro e altura)
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

// Função para validar a linha do cilindro
char *validate_cylinder(char *str, t_data *data)
{
    char *tmp;
    char *center_p = NULL;
    char *normal_p = NULL;
    char *diameter_p = NULL;
    char *height_p = NULL;
    char *rgb_p;
    char *out = NULL;
    int i = 0;
    int commas = 0;

    // Verifica se str é válido e começa com 'cy'
    if (!str || !strstr(str, "cy")) {
        return (NULL);
    }

    // Trim da parte após o 'cy'
    tmp = trim(strstr(str, "cy") + 2);
    
    // Calcula o tamanho da parte do vetor de centro
    while (tmp[i])
    {
        if (commas == 2 && ft_isalnum(tmp[i]))
        {
            while (tmp[i] && (ft_isalnum(tmp[i]) || tmp[i] == '_' || tmp[i] == '.'))
                i ++;
            break ;
        }
        if (tmp[i] == ',')
            commas ++;
        i ++;
    }
    if (commas != 2) {
        return (NULL);
    }
    
    // Aloca center_p
    center_p = malloc(sizeof(char) * (i + 1));
    if (!center_p) {
        return (NULL);
    }
    
    ft_strlcpy(center_p, tmp, i + 1);
    center_p[i] = '\0';
    
    // Trim da parte do vetor normal
    char *next = trim(tmp + i);
    i = 0;
    commas = 0;
    
    // Calcula o tamanho da parte do vetor normal
   while (next[i])
    {
        if (commas == 2 && ft_isalnum(next[i]))
        {
            while (next[i] && (ft_isalnum(next[i]) || next[i] == '_' || next[i] == '.'))
                i ++;
            break ;
        }
        if (next[i] == ',')
            commas ++;
        i ++;
    }
    if (commas != 2) {
        free(center_p);
        return (NULL);
    }
    
    // Aloca normal_p
    normal_p = malloc(sizeof(char) * (i + 1));
    if (!normal_p) {
        free(center_p);
        return (NULL);
    }
    
    ft_strlcpy(normal_p, next, i + 1);
    normal_p[i] = '\0';
    
    // Trim da parte do diâmetro
    next = trim(next + i);
    i = 0;
    
    // Calcula o tamanho da parte do diâmetro
    while (next[i] && !isspace(next[i])) i++;
    
    // Aloca diameter_p
    diameter_p = malloc(sizeof(char) * (i + 1));
    if (!diameter_p) {
        free(center_p);
        free(normal_p);
        return (NULL);
    }
    
    ft_strlcpy(diameter_p, next, i + 1);
    diameter_p[i] = '\0';
    
    // Trim da parte da altura
    next = trim(next + i);
    i = 0;
    
    // Calcula o tamanho da parte da altura
    while (next[i] && !isspace(next[i])) i++;
    
    // Aloca height_p
    height_p = malloc(sizeof(char) * (i + 1));
    if (!height_p) {
        free(center_p);
        free(normal_p);
        free(diameter_p);
        return (NULL);
    }
    
    ft_strlcpy(height_p, next, i + 1);
    height_p[i] = '\0';
    
    // Trim da parte do RGB
    rgb_p = trim(next + i);
    
    // Valida os componentes
    if (is_valid_vector3d(center_p) && is_valid_vector3d(normal_p) && 
        is_valid_positive_double(diameter_p) && is_valid_positive_double(height_p) && 
        is_valid_rgb(rgb_p))
    {
        // Constrói a string de saída
        out = ft_strdup("cy ");
        if (!out) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            return (NULL);
        }
        
        char *temp = ft_strjoin(out, removeEspacosETabs(center_p));
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(str);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(normal_p));
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(diameter_p));
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(height_p));
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(rgb_p));
        if (!temp) {
            free(center_p);
            free(normal_p);
            free(diameter_p);
            free(height_p);
            return (NULL);
        }
        out = temp;
        
        printf("Dados certos!\n\n");
        printf("FINAL - %s\n", out);
    }
    else
    {
        data->invalid_line = 1;
        free(center_p);
        free(normal_p);
        free(diameter_p);
        free(height_p);
        return (NULL);
    }
    
    // Libera memória
    free(center_p);
    free(normal_p);
    free(diameter_p);
    free(height_p);
    
    return (out);
}