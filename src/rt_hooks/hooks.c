/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:41:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/11 09:13:03 by manandre         ###   ########.fr       */
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
    free_mem(get_mem_address());              // Libera memória alocada
    mlx_destroy_window(data->mlx, data->win); // Destrói a janela
    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx, data->img.img_ptr); // Destrói a imagem
    if (data->mlx)
        mlx_destroy_display(data->mlx); // Destrói o display do mlx
    free_list(&data->sphere_l);         // Libera a lista de esferas
    free_list(&data->cylinder_l);       // Libera a lista de cilindros
    free(data->mlx);                    // Libera a memória do mlx
    printf("Janela fechada e memória liberada.\n");
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
int mouse_click(int keycode, int x, int y, t_data *data)
{
    (void)data;       // Evita aviso de variável não utilizada
    if (keycode == 1) // Botão esquerdo do mouse
    {
        t_list *list;
        t_sphere *sphere;
        list = data->sphere_l;
        while (list)
        {
            t_sphere *sphere = (t_sphere *)list->content;
            if (sphere->clicked == 1)
            {
                data->
                break;
            }

            list = list->next;
        }

        // Aqui você pode implementar lógica para lidar com o clique do mouse
        // Por exemplo, você pode adicionar um objeto na posição do clique
        printf("Mouse clicked at: (%d, %d)\n", x, y);
    }
    return (0);
}

// Função de callback para o evento de movimento da câmera com as teclas a, s, d, w
int camera_move(int keycode, t_data *data)
{
    if (keycode == 119)                // W key
        data->camera->origin.z += 0.5; // Move the camera forward
    else if (keycode == 115)           // S key
        data->camera->origin.z -= 0.5; // Move the camera backward
    else if (keycode == 100)           // D key
        data->camera->origin.x += 0.5; // Move the camera right
    else if (keycode == 97)            // A key
        data->camera->origin.x -= 0.5; // Move the camera left
    else if (keycode == 101)           // E key
        data->camera->origin.y += 0.5; // Move the camera up
    else if (keycode == 113)           // Q key
        data->camera->origin.y -= 0.5; // Move the camera down
    render_scene(data);                // Re-render the scene after moving the camera
    return (0);
}

// Função de callback para o evento de movimento da object com as teclas v, b
int object_move(int keycode, t_data *data)
{
    t_sphere *sph = (t_sphere *)data->sphere_l->content;
    if (keycode == 118)
        sph->center.x += 0.5; // V key
    else if (keycode == 98)
        sph->center.x -= 0.5; // B key
    else if (keycode == 116)
        sph->center.z += 0.5; // T key - tras
    else if (keycode == 103)
        sph->center.z -= 0.5; // G key  - frente
    else if (keycode == 114)
        sph->center.y += 0.5; // T key - cima
    else if (keycode == 102)
        sph->center.y -= 0.5; // G key  - baixo

    render_scene(data); // Re-render the scene after moving the camera
    return (0);
}

// Função de callback para o evento de pressionamento de tecla, para testar a mudanca na posicao da luz)

int key_press(int keycode, t_data *data)
{
    printf(":::: %d\n", keycode);
    if (keycode == 65307)                                                                                                          // Escape key
        close_window(data);                                                                                                        // Fecha a janela
    else if (keycode == 65361 || keycode == 65363 || keycode == 65362 || keycode == 65364 || keycode == 65451 || keycode == 65453) // Setas e Page Up/Down
        light_move(keycode, data);                                                                                                 // Move a luz
    else if (keycode == 119 || keycode == 115 || keycode == 97 || keycode == 100 || keycode == 101 || keycode == 113)              // W, S, A, D, Q, E keys
    {
        printf(":::: came move\n");
        camera_move(keycode, data); // Move a câmera
    }
    else if (keycode == 65470) // F5 key
        render_scene(data);    // Re-renderiza a cena
    else if (keycode == 118 || keycode == 98 || keycode == 116 || keycode == 103 || keycode == 114 || keycode == 102)
    {
        object_move(keycode, data); // mover o objecto
    }
    /* else if (keycode == 65474) // F6 key
     {
         mlx_destroy_image(data->mlx, data->img.img_ptr); // Destrói a imagem atual
         init_image(&data->img, data->mlx, data->img.width, data->img.height); // Re-inicializa a imagem
         render_scene(data); // Re-renderiza a cena
     }*/
    return (0);
}
