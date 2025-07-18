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

// Função para validar a linha do plano
char *validate_plane(char *str, t_data *data)
{
    char *tmp;
    char *position_p = NULL;
    char *normal_p = NULL;
    char *rgb_p;
    char *out = NULL;
    int i = 0;
    int commas = 0;

    // Trim da parte após o 'pl'
    tmp = trim(strstr(str, "pl") + 2);
    
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
    
    // Trim da parte do vetor normal
    char *next = trim(tmp + i);
    i = 0;
    commas = 0;
    
    // Calcula o tamanho da parte do vetor normal
   while (next[i])
    {
        if (commas == 2 && ft_isalnum(next[i]))
        {
            while (tmp[i] && (ft_isalnum(next[i]) || next[i] == '_' || next[i] == '.'))
                i ++;
            break ;
        }
        if (next[i] == ',')
            commas ++;
        i ++;
    }
    if (commas != 2) {
        free(position_p);
        return (NULL);
    }
    
    // Aloca normal_p
    normal_p = malloc(sizeof(char) * (i + 1));
    if (!normal_p) {
        free(position_p);
        return (NULL);
    }
    
    ft_strlcpy(normal_p, next, i + 1);
    normal_p[i] = '\0';
    
    // Trim da parte do RGB
    rgb_p = trim(next + i);
    
    // Valida os componentes
    if (is_valid_vector3d(position_p) && is_valid_vector3d(normal_p) && is_valid_rgb(rgb_p))
    {
        // Constrói a string de saída
        out = ft_strdup("pl ");
        if (!out) {
            free(position_p);
            free(normal_p);
            return (NULL);
        }
        
        char *temp = ft_strjoin(out, removeEspacosETabs(position_p));
        if (!temp) {
            free(position_p);
            free(normal_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(position_p);
            free(normal_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(normal_p));
        if (!temp) {
            free(position_p);
            free(normal_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, " ");
        if (!temp) {
            free(position_p);
            free(normal_p);
            free(out);
            return (NULL);
        }
        out = temp;
        
        temp = ft_strjoin(out, removeEspacosETabs(rgb_p));
        if (!temp) {
            free(position_p);
            free(normal_p);
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
        free(normal_p);
        return (NULL);
    }
    
    // Libera memória
    free(position_p);
    free(normal_p);
    
    return (out);
}