/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:41:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/09 12:41:52 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// Função de callback para o evento de movimento da luz com as teclas de seta
int light_move(int keycode, t_data *data)
{
    if (keycode == 65361) // Left arrow key
        data->light->position.x -= 0.5;
    else if (keycode == 65363) // Right arrow key
        data->light->position.x += 0.5;
    else if (keycode == 65362) // Up arrow key
        data->light->position.y += 0.5;
    else if (keycode == 65364) // Down arrow key
        data->light->position.y -= 0.5;
    else if (keycode == 65451) // Page Up key
        data->light->position.z += 0.5;
    else if (keycode == 65453) // Page Down key
        data->light->position.z -= 0.5;
    render_scene(data);
    return (0);
}

// Função de callback para o evento de fechamento da janela
int close_window(t_data *data)
{
    free_all(&data->garbage); // Libera todos os objetos alocados
    exit(0);
}

// Função de callback para o evento de redimensionamento da janela
int resize_window(int width, int height, t_data *data)
{
    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx, data->img.img_ptr);
    init_image(&data->img, data->mlx, width, height);
    mlx_clear_window(data->mlx, data->win);
    render_scene(data);
    return (0);
}

// Função de callback para o evento de movimento do mouse
int mouse_move(int x, int y, t_data *data)
{
    (void)data; // Evita aviso de variável não utilizada
    // Aqui você pode implementar lógica para atualizar a cena com base na posição do mouse
    // Por exemplo, você pode atualizar a posição da câmera ou de um objeto com base na posição do mouse
    // Neste exemplo, apenas imprimimos a posição do mouse
    printf("Mouse moved to: (%d, %d)\n", x, y);
    return (0);
}

// Função de callback para o evento de clique do mouse
int mouse_click(int button, int x, int y, t_data *data)
{
    (void)data; // Evita aviso de variável não utilizada
    if (button == 1) // Botão esquerdo do mouse         
    {
        // Aqui você pode implementar lógica para lidar com o clique do mouse
        // Por exemplo, você pode adicionar um objeto na posição do clique
        printf("Mouse clicked at: (%d, %d)\n", x, y);
    }
    return (0);
}   

// Função de callback para o evento de movimento da câmera com as teclas a, s, d, w
int camera_move(int keycode, t_data *data)
{
    if (keycode == 119) // W key
        data->camera->origin.z += 0.5; // Move the camera forward
    else if (keycode == 115) // S key
        data->camera->origin.z -= 0.5; // Move the camera backward
    else if (keycode == 100) // D key
        data->camera->origin.x += 0.5; // Move the camera right
    else if (keycode == 97) // A key
        data->camera->origin.x -= 0.5; // Move the camera left
    else if (keycode == 101) // E key
        data->camera->origin.y += 0.5; // Move the camera up
    else if (keycode == 113) // Q key
        data->camera->origin.y -= 0.5; // Move the camera down
    render_scene(data); // Re-render the scene after moving the camera
    return (0);
}

// Função de callback para o evento de pressionamento de tecla, para testar a mudanca na posicao da luz)

int key_press(int keycode, t_data *data)
{
    if (keycode == 65307) // Escape key
        close_window(data); // Fecha a janela
    else if (keycode == 65361 || keycode == 65363 
            || keycode == 65362 || keycode == 65364
            || keycode == 65451 || keycode == 65453) // Setas e Page Up/Down
        light_move(keycode, data); // Move a luz
    else if (keycode == 119 || keycode == 115 
            || keycode == 97 || keycode == 100
            || keycode == 101 || keycode == 113) // W, S, A, D, Q, E keys
        camera_move(keycode, data); // Move a câmera
    else if (keycode == 65470) // F5 key
        render_scene(data); // Re-renderiza a cena
   /* else if (keycode == 65474) // F6 key
    {
        mlx_destroy_image(data->mlx, data->img.img_ptr); // Destrói a imagem atual
        init_image(&data->img, data->mlx, data->img.width, data->img.height); // Re-inicializa a imagem
        render_scene(data); // Re-renderiza a cena
    }*/
    return (0);
}
