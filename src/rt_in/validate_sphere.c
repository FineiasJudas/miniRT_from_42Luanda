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

// Função para validar a linha da luz difusa
char *validate_sphere(char *str, t_data *data)
{
    char *tmp;
    char *position_p = NULL;
    char *ratio = NULL;
    char *rgb_p;
    char *out = NULL;
    int i = 0;
    int commas = 0;

    // Trim da parte após o 'L'
    tmp = trim(strstr(str, "sp") + 2);
    
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
        free(str);
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
    ratio = malloc(sizeof(char) * (i + 1));
    if (!ratio) {
        free(position_p);
        return (NULL);
    }
    
    ft_strlcpy(ratio, next, i + 1);
    ratio[i] = '\0';
    
    // Trim da parte do RGB
    rgb_p = trim(next + i);
    
    // Valida os componentes
    if (is_valid_vector3d(position_p) && is_valid_double(ratio) && is_valid_rgb(rgb_p))
    {
        // Constrói a string de saída
        out = ft_strdup("L ");
        if (!out) {
            free(position_p);
            free(ratio);
            return (NULL);
        }
        
        char *temp = ft_strjoin(out, removeEspacosETabs(position_p));
        if (!temp) {
            free(position_p);
            free(ratio);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(position_p);
            free(ratio);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(ratio));
        if (!temp) {
            free(position_p);
            free(ratio);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(position_p);
            free(ratio);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(rgb_p));
        if (!temp) {
            free(position_p);
            free(ratio);
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
        free(ratio);
        return (NULL);
    }
    
    // Libera memória
    free(position_p);
    free(ratio);
    
    return (out);
}
