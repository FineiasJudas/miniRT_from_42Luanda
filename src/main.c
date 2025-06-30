/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 14:04:45 by fjilaias         ###   ########.fr       */
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

/*FIM RENDER*/

// Função de callback para o evento de pressionamento de tecla, para testar a mudanca na posicao da luz)
int key_press(int keycode, t_data *data)
{
    printf("Nova luz.z = %d\n", keycode);

    if (keycode == 65362)
    {
        // cima z
        data->light->position.z = fmod(data->light->position.z + 1, 100);
        printf("Nova luz.z = %f\n", data->light->position.z);
    }
    else if (keycode == 65361)
    {
        // esquerda x
        data->light->position.x = fmod(data->light->position.x + 1, 100);
        printf("Nova luz.x = %f\n", data->light->position.x);
    }
    else if (keycode == 65364)
    {
        // baixo y
        data->light->position.y = fmod(data->light->position.y + 1, 200);
        printf("Nova luz.y = %f\n", data->light->position.y);
    }

    // mudar a posicao da camera com w-a-s
    else if (keycode == 119)
    {
        data->camera->origin.z = fmod(data->camera->origin.z + 0.5, 25);
    }
    else if (keycode == 97)
    {
        data->camera->origin.y = fmod(data->camera->origin.y + 0.5, 25);
    }
    else if (keycode == 115)
    {
        data->camera->origin.x = fmod(data->camera->origin.x + 0.5, 25);
    }
//    render_scene_cube(data); // primeira renderização
	render_scene(data); // redesenha com nova posição
    return 0;
}

int main(int ac, char **av)
{
    t_data  data;

    create_data(&data);
    if (ac == 2)
        parse_rt_file(av[1], &data);
    data.render = (t_render *)malloc(sizeof(t_render));
    prin_data(&data);
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, 800, 600, "miniRT");
    init_image(&data.img, data.mlx, 800, 600);
    render_scene(&data); // primeira renderização
    mlx_hook(data.win, 2, 1L << 0, key_press, &data); // eventos de tecla
    mlx_loop(data.mlx);
    return 0;
}
