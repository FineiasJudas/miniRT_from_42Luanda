/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:41:31 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/11 16:47:01 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

// Função de callback para o evento de movimento da luz com as teclas de seta
int light_move(int keycode, t_light *light, t_data *data)
{
    if (keycode == 65361) // Left arrow key
        light->position.x -= 0.5;
    else if (keycode == 65363) // Right arrow key
        light->position.x += 0.5;
    else if (keycode == 65362) // Up arrow key
        light->position.y += 0.5;
    else if (keycode == 65364) // Down arrow key
        light->position.y -= 0.5;
    else if (keycode == 65451) // Page Up key
        light->position.z += 0.5;
    else if (keycode == 65453) // Page Down key
        light->position.z -= 0.5;
    render_scene(data);
    return (0);
}

// Função de callback para o evento de fechamento da janela
int close_window(t_data *data)
{
    free_mem(get_mem_address()); // Libera memória alocada
    mlx_destroy_window(data->mlx, data->win); // Destrói a janela
    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx, data->img.img_ptr); // Destrói a imagem
    if (data->mlx)
        mlx_destroy_display(data->mlx); // Destrói o display do mlx
    free_list(&data->sphere_l); // Libera a lista de esferas
    free_list(&data->cylinder_l); // Libera a lista de cilindros
    free(data->mlx); // Libera a memória do mlx
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
void select_next_object(t_data *data)
{
    printf("Selecionando próximo objeto...\n");
    // Avança para o próximo elemento da lista atual
    if (data->selected_type == SPHERE && data->selected_node && data->selected_node->next)
        data->selected_node = data->selected_node->next;
    else if (data->selected_type == SPHERE)
    {
        data->selected_type = CYLINDER;
        data->selected_node = data->cylinder_l;
    }
    else if (data->selected_type == CYLINDER && data->selected_node && data->selected_node->next)
        data->selected_node = data->selected_node->next;
    else if (data->selected_type == CYLINDER)
    {
        data->selected_type = LIGHT;
        data->selected_node = data->lights_l;
    }
    else if (data->selected_type == LIGHT && data->selected_node && data->selected_node->next)
        data->selected_node = data->selected_node->next;
    else
    {
        // Volta para o início das esferas
        data->selected_type = SPHERE;
        data->selected_node = data->sphere_l;
    }

    // Atualiza ponteiro do objeto
    if (!data->selected_node)
    {
        data->selected_type = NONE;
        data->selected_obj = NULL;
        printf("Nada selecionado\n");
        return;
    }

    data->selected_obj = data->selected_node->content;

    if (data->selected_type == SPHERE)
        printf("Selecionou esfera\n");
    else if (data->selected_type == CYLINDER)
        printf("Selecionou cilindro\n");
    else if (data->selected_type == LIGHT)
        printf("Selecionou luz\n");

    render_scene(data);
}


int is_any_object_selected(t_data *data)
{
    return (data->selected_type != NONE && data->selected_obj != NULL);
}

int sphere_move(int keycode, t_sphere *s, t_data *data)
{
    if (keycode == 65361) s->center.x -= 0.5;
    else if (keycode == 65363) s->center.x += 0.5;
    else if (keycode == 65362) s->center.y += 0.5;
    else if (keycode == 65364) s->center.y -= 0.5;
    else if (keycode == 65451) s->center.z += 0.5;
    else if (keycode == 65453) s->center.z -= 0.5;
    render_scene(data);
    return (0);
}


int cylinder_move(int keycode, t_cylinder *c, t_data *data)
{
    if (keycode == 65361) // Left arrow key
    {
        c->center.x -= 0.5; // Move o cilindro para a esquerda
        printf("Cylinder moved left to: (%f, %f, %f)\n", data->cylinder->center.x, data->cylinder->center.y, data->cylinder->center.z);
    }
    else if (keycode == 65363) // Right arrow key
    {
        c->center.x += 0.5; // Move o cilindro para a direita
        printf("Cylinder moved right to: (%f, %f, %f)\n", data->cylinder->center.x, data->cylinder->center.y, data->cylinder->center.z);
    }
    else if (keycode == 65362) // Up arrow key
    {
        c->center.y += 0.5; // Move o cilindro para cima
        printf("Cylinder moved up to: (%f, %f, %f)\n", data->cylinder->center.x, data->cylinder->center.y, data->cylinder->center.z);
    }
    else if (keycode == 65364) // Down arrow key
    {
        c->center.y -= 0.5; // Move o cilindro para baixo
        printf("Cylinder moved down to: (%f, %f, %f)\n", data->cylinder->center.x, data->cylinder->center.y, data->cylinder->center.z);
    }
    render_scene(data); // Re-renderiza a cena após mover o objeto
    return (0);
}

int key_press(int keycode, t_data *data)
{
    if (keycode == 65307) // ESC
        close_window(data);

    else if (keycode == 65289) // Tab
        select_next_object(data);

    else if ((keycode == 65361 || keycode == 65363
           ||  keycode == 65362 || keycode == 65364
           ||  keycode == 65451 || keycode == 65453))
    {
        if (!is_any_object_selected(data))
            return (0);

        if (data->selected_type == SPHERE)
            sphere_move(keycode, (t_sphere *)data->selected_obj, data);
        else if (data->selected_type == CYLINDER)
            cylinder_move(keycode, (t_cylinder *)data->selected_obj, data);
        else if (data->selected_type == LIGHT)
            light_move(keycode, (t_light *)data->selected_obj, data);
    }

    else if (keycode == 119 || keycode == 115 || keycode == 97 || keycode == 100 || keycode == 113 || keycode == 101)
        camera_move(keycode, data);

    return (0);
}

