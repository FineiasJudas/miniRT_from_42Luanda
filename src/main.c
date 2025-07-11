/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/11 08:33:18 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void    calculate_ray(int x, int y, t_data *data)
{
    float   ndc_x;
    float   ndc_y;

    // Converter coordenadas de pixel para espaço NDC
    ndc_x = (2 * (float)x / 800 - 1);
    ndc_y = (1 - 2 * (float)y / 600);

    // Definir a direção do raio
    data->ray.direction = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
    data->ray.origin = data->camera->origin;
}

int is_a_rt_extension(char *filename)
{
    int len;

    len = ft_strlen(filename);
    if (len < 3)
        return (0);
    if (filename[len - 3] == '.' 
        && filename[len - 2] == 'r' 
        && filename[len - 1] == 't')
        return (1);
    fprintf(stderr, "O ficheiro %s não tem a extensão .rt\n", filename);
    fprintf(stderr, "Por favor, use um ficheiro com a extensão .rt\n");
    return (0);
}

int main(int ac, char **av)
{
    t_data  data;

     printf("::::");
    create_data(&data);
    if (ac == 2)
    {
        if (!is_a_rt_extension(av[1]) || parse_rt_file(av[1], &data) != 0
            || data.invalid_line)
            return (1);
    }
    data.render = (t_render *)malloc(sizeof(t_render));
    if (!data.render)
        return 1 * fprintf(stderr, "Erro ao alocar memória para render\n");
    collect_mem(data.render);
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, 800, 600, "miniRT");
    init_image(&data.img, data.mlx, 800, 600);
    render_scene(&data); // primeira renderização 
    mlx_hook(data.win, 2, 1L << 0, key_press, &data); // eventos de tecla
     mlx_mouse_hook(data.win, mouse_click, NULL);
    mlx_loop(data.mlx);
    return (0);
}
