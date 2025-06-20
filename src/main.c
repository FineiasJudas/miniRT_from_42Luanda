/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/20 08:50:50 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float angle = 0.0f;

/*FJILAIA-> CRIAR UM OU MAIS FICHEIRO PARA FUNCOES DE OPERACOES COM VECTORES */

// INICIO OPERACOES COM VECTORES
t_vector vec_sub(t_vector a, t_vector b)
{
    return (t_vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

float vec_dot(t_vector a, t_vector b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_vector vec_normalize(t_vector v)
{
    float len = sqrtf(vec_dot(v, v));
    if (len == 0)
        return (t_vector){0, 0, 0}; // evitar divisão por 0
    return (t_vector){v.x / len, v.y / len, v.z / len};
}

t_vector vec_scale(t_vector v, float s)
{
    return (t_vector){v.x * s, v.y * s, v.z * s};
}

t_vector vec_add(t_vector a, t_vector b)
{
    return (t_vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

// FIM OPERACOES COM VECTORES

t_color scale_color(t_color color, float factor)
{
    return (t_color){fmin(color.r * factor, 255),
                     fmin(color.g * factor, 255),
                     fmin(color.b * factor, 255)};
}

/*
INICIO INTERACAO
intercesao dos raios da camera com os objectos
colocar todos num ficheiros ou varios(regra das 5 funcoes)
*/

int intersect_ray_sphere(t_ray ray, t_sphere *s, float *t)
{
    t_vector oc = vec_sub(ray.origin, s->center);
    float a = vec_dot(ray.direction, ray.direction);
    float b = 2.0 * vec_dot(oc, ray.direction);
    float c = vec_dot(oc, oc) - s->radius * s->radius;
    float disc = b * b - 4 * a * c;

    //t_vector hit_point = vec_add(ray.origin, vec_scale(ray.direction, *t));

    if (disc < 0)
        return 0;
    *t = (-b - sqrtf(disc)) / (2.0 * a);
    return (*t > 0);
}

double intersect_plane(t_vector *ray_origin, t_vector *ray_dir, t_plane *plane)
{
    t_vector origin_to_plane = { ray_origin->x - plane->coordinates.x, ray_origin->y - plane->coordinates.y, ray_origin->z - plane->coordinates.z };
    double numerator = vec_dot(plane->normalized, origin_to_plane);
    double denominator = vec_dot(plane->normalized, *ray_dir);
    if (fabs(denominator) < 0.0001)
        return -1.0;
    double t = -numerator / denominator;
    if (t > 0.0001)
        return t;
    return -1.0;
}


/*int intersect_ray_plane(t_ray ray, t_plane *p, float *t)
{
}
int intersect_ray_cylinder(t_ray ray, t_cylinder *c, float *t)
{
}*/

int intersect_ray_cube(t_ray ray, t_cube *cube, float *t)
{
    float tmin = (cube->min.x - ray.origin.x) / ray.direction.x;
    float tmax = (cube->max.x - ray.origin.x) / ray.direction.x;
    if (tmin > tmax)
    {
        float tmp = tmin;
        tmin = tmax;
        tmax = tmp;
    }

    float tymin = (cube->min.y - ray.origin.y) / ray.direction.y;
    float tymax = (cube->max.y - ray.origin.y) / ray.direction.y;
    if (tymin > tymax)
    {
        float tmp = tymin;
        tymin = tymax;
        tymax = tmp;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return 0;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (cube->min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (cube->max.z - ray.origin.z) / ray.direction.z;
    if (tzmin > tzmax)
    {
        float tmp = tzmin;
        tzmin = tzmax;
        tzmax = tmp;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return 0;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    if (tmax < 0)
        return 0;

    *t = (tmin > 0) ? tmin : tmax;
    return 1;
}

/*
MOVENDO OBJECTOS
*/

t_vector rotate_y(t_vector v, float angle)
{
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    return (t_vector){
        v.x * cos_a + v.z * sin_a,
        v.y,
        -v.x * sin_a + v.z * cos_a};
}

t_vector get_ray_direction(int x, int y, int width, int height) {
    t_vector dir;
    double aspect_ratio = (double)width / height;
    dir.x = (2.0 * x / width - 1.0) * aspect_ratio; // Normaliza x para [-aspect, aspect]
    dir.y = 1.0 - 2.0 * y / height;      // Normaliza y para [-1, 1]
    dir.z = -1.0;                                   // Direção Z (para frente)
    return dir;
}

t_color calc_ambient(t_color *c, t_ambient *ambient)
{
    t_color out;

    out.r =(int) c->r * (ambient->color.r / 255.0) * ambient->ratio;
    out.g =(int) c->g * (ambient->color.g / 255.0) * ambient->ratio;
    out.b =(int) c->b * (ambient->color.b / 255.0) * ambient->ratio;
    
    if (out.r > 255) out.r = 255;
    if (out.g > 255) out.g = 255;
    if (out.b > 255) out.b = 255;
    if (out.r < 0) out.r = 0;
    if (out.g < 0) out.g = 0;
    if (out.b < 0) out.b = 0;

    return (out);
}

t_color ambient_light(t_color *src, double intensity, t_ambient *ambient)
{
    t_color dif;
    t_color amb;
    t_color out;

    dif = scale_color(*src, intensity);
    amb = calc_ambient(src, ambient);

    out.r = dif.r + amb.r;
    out.g = dif.g + amb.g;
    out.b = dif.b + amb.b;
    if (out.r > 255) out.r = 255;
    if (out.g > 255) out.g = 255;
    if (out.b > 255) out.b = 255;
    return (out);
}

/*FIM INTERACAO*/

/*INICIO DOS RENDER(DEPOIS REFACTORAR E TORNAR GENERICO)*/
void render_scene(t_data *data)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            // Converter coordenadas de pixel para espaço NDC
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);

            // Definir a direção do raio
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
            t_ray ray = {data->camera->origin, ray_dir};
            // Calcular interseções
            
            float t_sphere = -1.0f;
            double t_plane = intersect_plane(&data->camera->origin, &ray_dir, data->plane);
            int hit_sphere = intersect_ray_sphere(ray, data->sphere, &t_sphere);
            

            if (hit_sphere && t_sphere > 0 && (t_plane < 0 || t_sphere < t_plane))
            {
                // Renderizar a esfera
                t_vector hit = vec_add(ray.origin, vec_scale(ray.direction, t_sphere));
                t_vector normal = vec_normalize(vec_sub(hit, data->sphere->center));
                t_vector light_dir = vec_normalize(vec_sub(data->light->position, hit));
                
                // Verificar se está na sombra
                int in_shadow = 0;
                t_ray shadow_ray = {vec_add(hit, vec_scale(normal, 0.001)), light_dir}; // Pequeno offset para evitar auto-interseção
                float t_shadow;
                double t_plane_shadow = intersect_plane(&shadow_ray.origin, &shadow_ray.direction, data->plane);
                if (intersect_ray_sphere(shadow_ray, data->sphere, &t_shadow) && t_shadow < vec_dot(vec_sub(data->light->position, hit), vec_sub(data->light->position, hit)))
                    in_shadow = 1;
                else if (t_plane_shadow > 0 && t_plane_shadow < vec_dot(vec_sub(data->light->position, hit), vec_sub(data->light->position, hit)))
                    in_shadow = 1;
                
                // Calcular intensidade
                float diffuse_intensity = in_shadow ? 0.0 : fmax(0.0, vec_dot(normal, light_dir));
                t_color color = ambient_light(&data->sphere->color, diffuse_intensity, data->ambient);
                put_pixel(&data->img, x, y, color);
            }
            else if (t_plane > 0)
            {
                // Renderizar o plano
                t_vector hit = vec_add(ray.origin, vec_scale(ray.direction, t_plane));
                t_vector light_dir = vec_normalize(vec_sub(data->light->position, hit));
                
                // Verificar se está na sombra
                int in_shadow = 0;
                t_ray shadow_ray = {vec_add(hit, vec_scale(data->plane->normalized, 0.001)), light_dir};
                float t_shadow;
                if (intersect_ray_sphere(shadow_ray, data->sphere, &t_shadow) && t_shadow < vec_dot(vec_sub(data->light->position, hit), vec_sub(data->light->position, hit)))
                    in_shadow = 1;
                // Não verificamos o plano novamente, pois o raio de sombra já está acima dele
                
                // Calcular intensidade
                float diffuse_intensity = in_shadow ? 0.0 : fmax(0.0, vec_dot(data->plane->normalized, light_dir));
                t_color color = ambient_light(&data->plane->color, diffuse_intensity, data->ambient);
                put_pixel(&data->img, x, y, color);
            }
            else
            {
                // Sem interseção
                put_pixel(&data->img, x, y, (t_color){19, 24, 33});
            }
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

// CUBO

t_vector calculate_cube_normal(t_vector hit, t_cube *cube)
{
    const float epsilon = 0.001f;

    if (fabs(hit.x - cube->min.x) < epsilon)
        return (t_vector){-1, 0, 0};
    else if (fabs(hit.x - cube->max.x) < epsilon)
        return (t_vector){1, 0, 0};
    else if (fabs(hit.y - cube->min.y) < epsilon)
        return (t_vector){0, -1, 0};
    else if (fabs(hit.y - cube->max.y) < epsilon)
        return (t_vector){0, 1, 0};
    else if (fabs(hit.z - cube->min.z) < epsilon)
        return (t_vector){0, 0, -1};
    else if (fabs(hit.z - cube->max.z) < epsilon)
        return (t_vector){0, 0, 1};

    // Caso raro: ponto no meio de uma aresta — escolhe normal nula (ou trata de outra forma)
    return (t_vector){0, 0, 0};
}

void render_scene_cube(t_data *data)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            float ndc_x = (2 * (float)x / 800 - 1);
            float ndc_y = (1 - 2 * (float)y / 600);
            t_vector ray_dir = vec_normalize((t_vector){ndc_x * 2, ndc_y * 1.5, -2});
            t_ray ray = {data->camera->origin, ray_dir};
            float t;

            // rotacionar origem e direção do raio
            angle += 0.01f; // gira um pouco a cada frame

            t_ray transformed_ray;
            (void)transformed_ray;
            transformed_ray.origin = rotate_y(ray.origin, -angle);
            transformed_ray.direction = rotate_y(ray.direction, -angle);

            if (intersect_ray_cube(ray, data->cube, &t))
            {
                t_vector hit = vec_add(ray.origin, vec_scale(ray.direction, t));

                // Calcula normal do cubo(em world space, depois da rotação)
                t_vector local_hit = rotate_y(hit, -angle); // ponto no sistema local

                // Calcular normal baseado na face atingida (simplificado)

                t_vector normal = calculate_cube_normal(local_hit, data->cube);
                normal = rotate_y(normal, angle); // volta a normal pro mundo

                t_vector light_dir = vec_normalize(vec_sub(data->light->position, hit));
                float intensity = fmax(0.1, vec_dot(normal, light_dir));
                t_color color = scale_color(data->cube->color, intensity);
                put_pixel(&data->img, x, y, color);
            }
            else
            {
                put_pixel(&data->img, x, y, (t_color){0, 0, 0});
            }
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
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

    if (ac == 2)
        parse_rt_file(av[1], &data);
    prin_data(&data);
    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, 800, 600, "miniRT");
    init_image(&data.img, data.mlx, 800, 600);

    //data->light->position = (t_vector){2, 4, 0};
    //data->light->position = (t_vector){0, 4, -1};

    //data.cam_origin = &(t_vector){0, 0, 0}; // nao usar

    // data.cube.min = (t_vector){-1, -1, -4};
    // data.cube.max = (t_vector){1, 1, -2};
    // data.cube.color = (t_color){255, 255, 255};

    //data.sphere = &(t_sphere){{0, 0, -3}, 1.6, {255, 25, 255}};
    //render_scene_cube(&data); // primeira renderização

    render_scene(&data); // primeira renderização
    mlx_hook(data.win, 2, 1L << 0, key_press, &data); // eventos de tecla
    mlx_loop(data.mlx);
    return 0;
}
