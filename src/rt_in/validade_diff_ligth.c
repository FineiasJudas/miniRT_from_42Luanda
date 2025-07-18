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

// Função para validar o brilho (deve ser um double entre 0.0 e 1.0)
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

// Função para validar a linha da luz difusa
char *validate_light(char *str, t_data *data)
{
    char *tmp;
    char *position_p = NULL;
    char *brightness_p = NULL;
    char *rgb_p;
    char *out = NULL;
    int i = 0;
    int commas = 0;

    // Verifica se str é válido e começa com 'L'
    if (!str || !strchr(str, 'L')) {
        free(str);
        return (NULL);
    }

    // Trim da parte após o 'L'
    tmp = trim(strchr(str, 'L') + 1);
    
    // Calcula o tamanho da parte do vetor de posição
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
    
    // Aloca position_p
    position_p = malloc(sizeof(char) * (i + 1));
    if (!position_p) {
        return (NULL);
    }
    
    ft_strlcpy(position_p, tmp, i + 1);
    position_p[i] = '\0';
    
    // Trim da parte do brilho
    char *next = trim(tmp + i);
    i = 0;
    
    // Calcula o tamanho da parte do brilho
    while (next[i] && !isspace(next[i])) i++;
    
    // Aloca brightness_p
    brightness_p = malloc(sizeof(char) * (i + 1));
    if (!brightness_p) {
        free(position_p);
        return (NULL);
    }
    
    ft_strlcpy(brightness_p, next, i + 1);
    brightness_p[i] = '\0';
    
    // Trim da parte do RGB
    rgb_p = trim(next + i);
    
    // Valida os componentes
    if (is_valid_vector3d(position_p) && is_valid_brightness(brightness_p) && is_valid_rgb(rgb_p))
    {
        // Constrói a string de saída
        out = ft_strdup("L ");
        if (!out) {
            free(position_p);
            free(brightness_p);
            return (NULL);
        }
        
        char *temp = ft_strjoin(out, removeEspacosETabs(position_p));
        if (!temp) {
            free(position_p);
            free(brightness_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(position_p);
            free(brightness_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(brightness_p));
        if (!temp) {
            free(position_p);
            free(brightness_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(position_p);
            free(brightness_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(rgb_p));
        if (!temp) {
            free(position_p);
            free(brightness_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        printf("Dados certos!\n\n");
        printf("FINAL - %s\n", out);
    }
    else
    {
        data->invalid_line = 1;
        free(position_p);
        free(brightness_p);
        return (NULL);
    }
    
    // Libera memória
    free(position_p);
    free(brightness_p);
    
    return (out);
}
