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

t_vector rotate_vector(t_vector v, char axis, double angle)
{
    t_vector r;
    double cos_a = cos(angle);
    double sin_a = sin(angle);

    if (axis == 'x')
    {
        r.x = v.x;
        r.y = v.y * cos_a - v.z * sin_a;
        r.z = v.y * sin_a + v.z * cos_a;
    }
    else if (axis == 'y')
    {
        r.x = v.x * cos_a + v.z * sin_a;
        r.y = v.y;
        r.z = -v.x * sin_a + v.z * cos_a;
    }
    else if (axis == 'z')
    {
        r.x = v.x * cos_a - v.y * sin_a;
        r.y = v.x * sin_a + v.y * cos_a;
        r.z = v.z;
    }
    return r;
}

int camera_move(int keycode, t_data *data)
{
    double angle = 0.087; // ~5 graus em radianos

    if (keycode == 119) // W
        data->camera->origin.z += 0.5;
    else if (keycode == 115) // S
        data->camera->origin.z -= 0.5;
    else if (keycode == 100) // D
        data->camera->origin.x += 0.5;
    else if (keycode == 97) // A
        data->camera->origin.x -= 0.5;
    else if (keycode == 101) // E
        data->camera->origin.y += 0.5;
    else if (keycode == 113) // Q
        data->camera->origin.y -= 0.5;

    // ROTACIONAR CÂMERA
    else if (keycode == 'j') // Rotaciona para a esquerda (em torno do eixo Y)
        data->camera->direction = rotate_vector(data->camera->direction, 'y', angle);
    else if (keycode == 'l') // Rotaciona para a direita (em torno do eixo Y)
        data->camera->direction = rotate_vector(data->camera->direction, 'y', -angle);
    else if (keycode == 'i') // Olha para cima (em torno do eixo X)
        data->camera->direction = rotate_vector(data->camera->direction, 'x', angle);
    else if (keycode == 'k') // Olha para baixo (em torno do eixo X)
        data->camera->direction = rotate_vector(data->camera->direction, 'x', -angle);

    // Normaliza para evitar distorções acumuladas
    data->camera->direction = vec_normalize(data->camera->direction);

    printf("Camera pos: (%.2f, %.2f, %.2f), direção: (%.2f, %.2f, %.2f)\n",
        data->camera->origin.x, data->camera->origin.y, data->camera->origin.z,
        data->camera->direction.x, data->camera->direction.y, data->camera->direction.z);

    render_scene(data);
    return 0;
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
        data->selected_type = PLANE;
        data->selected_node = data->plane_l;
    }
    else if (data->selected_type == PLANE && data->selected_node && data->selected_node->next)
        data->selected_node = data->selected_node->next;
    else if (data->selected_type == PLANE)
    {
        data->selected_type = LIGHT;
        data->selected_node = data->lights_l;
    }
    else if (data->selected_type == LIGHT && data->selected_node && data->selected_node->next)
        data->selected_node = data->selected_node->next;
    else
    {
        data->selected_type = SPHERE;
        data->selected_node = data->sphere_l;
    }

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
    else if (data->selected_type == PLANE)
        printf("Selecionou plano\n");
    else if (data->selected_type == LIGHT)
        printf("Selecionou luz\n");

    render_scene(data);
}

int is_any_object_selected(t_data *data)
{
    return (data->selected_type != NONE && data->selected_obj != NULL);
}

int plane_move(int keycode, t_plane *p, t_data *data)
{
    if (keycode == 65361) // ←
        p->coordinates.x -= 0.5;
    else if (keycode == 65363) // →
        p->coordinates.x += 0.5;
    else if (keycode == 65362) // ↑
        p->coordinates.y += 0.5;
    else if (keycode == 65364) // ↓
        p->coordinates.y -= 0.5;
    else if (keycode == 65451) // PgUp
        p->coordinates.z += 0.5;
    else if (keycode == 65453) // PgDn
        p->coordinates.z -= 0.5;

    printf("Plano movido para: (%f, %f, %f)\n", p->coordinates.x, p->coordinates.y, p->coordinates.z);
    render_scene(data);
    return (0);
}


int sphere_move(int keycode, t_sphere *s, t_data *data)
{
    if (keycode == 65361) s->center.x -= 0.5;
    else if (keycode == 65363) s->center.x += 0.5;
    else if (keycode == 65362) s->center.y += 0.5;
    else if (keycode == 65364) s->center.y -= 0.5;
    else if (keycode == 'u') s->center.z += 0.5;
    else if (keycode == 'n') s->center.z -= 0.5;

    render_scene(data);
    return (0);
}

int cylinder_move(int keycode, t_cylinder *c, t_data *data)
{
    double angle = 0.087; // ~5 graus em radianos

    if (keycode == 65361) // Left
        c->center.x -= 0.5;
    else if (keycode == 65363) // Right
        c->center.x += 0.5;
    else if (keycode == 65362) // Up
        c->center.y += 0.5;
    else if (keycode == 65364) // Down
        c->center.y -= 0.5;
    else if (keycode == 'u') // Backward
        c->center.z -= 0.5;
    else if (keycode == 'n') // Forward
        c->center.z += 0.5;
    else if (keycode == 'x') // Rotação eixo X
        c->normalized = rotate_vector(c->normalized, 'x', angle);
    else if (keycode == 'y') // Rotação eixo Y
        c->normalized = rotate_vector(c->normalized, 'y', angle);
    else if (keycode == 'z') // Rotação eixo Z
        c->normalized = rotate_vector(c->normalized, 'z', angle);

    // Normaliza para evitar deformações acumuladas
    c->normalized = vec_normalize(c->normalized);

    printf("Cilindro em (%f, %f, %f), eixo: (%f, %f, %f)\n",
           c->center.x, c->center.y, c->center.z,
           c->normalized.x, c->normalized.y, c->normalized.z);

    render_scene(data);
    return 0;
}


int key_press(int keycode, t_data *data)
{
    if (keycode == 65307) // ESC
        close_window(data);

    else if (keycode == 65289) // Tab
        select_next_object(data);

    else if ((keycode == 65361 || keycode == 65363
       ||  keycode == 65362 || keycode == 65364
       ||  keycode == 'u' || keycode == 'n' || keycode == 'x' || keycode == 'y' || keycode == 'z'))
    {
        if (!is_any_object_selected(data))
            return (0);
        if (data->selected_type == SPHERE)
            sphere_move(keycode, (t_sphere *)data->selected_obj, data);
        else if (data->selected_type == CYLINDER)
            cylinder_move(keycode, (t_cylinder *)data->selected_obj, data);
        else if (data->selected_type == PLANE)
            plane_move(keycode, (t_plane *)data->selected_obj, data);
        else if (data->selected_type == LIGHT)
            light_move(keycode, (t_light *)data->selected_obj, data);
    }
    else if (keycode == 119 || keycode == 115 || keycode == 97 || keycode == 100 || keycode == 113 || keycode == 101
    || keycode == 'j' || keycode == 'l' || keycode == 'i' || keycode == 'k')
        camera_move(keycode, data);

    return (0);
}

