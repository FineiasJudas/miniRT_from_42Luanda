/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/27 15:09:05 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#define EPS 1e-4
#define INF 1e30
#define EPS 1e-4
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

t_vector cylinder_normal(t_cylinder cylinder, t_vector hit_point)
{
    // Vetor do centro do cilindro ao ponto de interseção
    t_vector center_to_hit = vec_sub(hit_point, cylinder.center);
    
    // Projeção deste vetor no eixo do cilindro
    double projection = vec_dot(center_to_hit, cylinder.normalized);
    t_vector projected_point = vec_add(cylinder.center, vec_scale(cylinder.normalized, projection));
    
    // A normal é o vetor do ponto projetado no eixo até o ponto de interseção
    t_vector normal = vec_sub(hit_point, projected_point);
    
    return vec_normalize(normal);
}

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

double intersect_ray_plane(t_vector *ray_origin, t_vector *ray_dir, t_plane *plane)
{
    t_vector origin_to_plane = vec_sub(*ray_origin, plane->coordinates);
    double numerator = vec_dot(plane->normalized, origin_to_plane);
    double denominator = vec_dot(plane->normalized, *ray_dir);
    if (fabs(denominator) < 0.0001)
        return -1.0;
    double t = -numerator / denominator;
    if (t > 0.0001)
        return t;
    return -1.0;
}

void get_ray_direction(int x, int y, int width, int height, t_data *data)
{
    double  aspect_ratio;

    aspect_ratio = (double)width / height;
    data->ray.direction.x = (2.0 * x / width - 1.0) * aspect_ratio; // Normaliza x para [-aspect, aspect]
    data->ray.direction.y = 1.0 - 2.0 * y / height;      // Normaliza y para [-1, 1]
    data->ray.direction.z = -1.0;                           // Direção Z (para frente).
    data->ray.origin = data->camera->origin;
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

int intersect_cylinder(t_ray ray, t_cylinder cyl, double *t_out)
{
    double  r     = cyl.diameter / 2.0;
    double  halfh = cyl.height   / 2.0;
    t_vector D    = cyl.normalized;           
    t_vector CO   = vec_sub(ray.origin, cyl.center);
    t_vector V    = ray.direction;

    // projeta V e CO no plano perpendicular a D
    t_vector m = vec_sub(V,     vec_scale(D, vec_dot(V, D)));
    t_vector n = vec_sub(CO,    vec_scale(D, vec_dot(CO, D)));

    double a    = vec_dot(m, m);
    double b    = 2.0 * vec_dot(m, n);
    double c    = vec_dot(n, n) - r*r;
    double disc = b*b - 4*a*c;
    if (disc < 0.0 || fabs(a) < EPS) 
        return 0;

    double sd   = sqrt(disc);
    double t0   = (-b - sd) / (2.0 * a);
    double t1   = (-b + sd) / (2.0 * a);
    double t_lat = INF;

    if (t0 > EPS) t_lat = t0;
    if (t1 > EPS && t1 < t_lat) t_lat = t1;

    int    hit   = 0;
    double t_min = INF;

    // 1) Verifica lateral
    if (t_lat < INF)
    {
        t_vector P = vec_add(ray.origin, vec_scale(V, t_lat));
        double y = vec_dot(D, vec_sub(P, cyl.center));
        if (y >= -halfh && y <= +halfh)
        {
            t_min = t_lat;
            hit   = 1;
        }
    }

    // 2) Verifica tampas
    double denom = vec_dot(V, D);
    if (fabs(denom) > EPS)
    {
        for (int s = -1; s <= 1; s += 2)
        {
            double cap_h = s * halfh;
            t_vector cc = vec_add(cyl.center, vec_scale(D, cap_h));
            double tc = vec_dot(vec_sub(cc, ray.origin), D) / denom;
            if (tc <= EPS) 
                continue;

            t_vector Pcap = vec_add(ray.origin, vec_scale(V, tc));
            if (vec_dot(vec_sub(Pcap, cc), vec_sub(Pcap, cc)) <= r*r && tc < t_min)
            {
                t_min = tc;
                hit   = 1;
            }
        }
    }

    if (hit)
    {
        *t_out = t_min;
        return 1;
    }
    return 0;
}

/*FIM INTERACAO*/
/*INICIO DOS RENDER(DEPOIS REFACTORAR E TORNAR GENERICO)*/

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
void cylinder_shadow_check(t_render *render, t_data *data)
{
    float t_sphere_shadow;
    double t_plane_shadow;
    double t_cylinder_shadow;
    int in_shadow = 0;

    // Raio de sombra com pequeno offset na direção da normal
    t_ray shadow_ray;
    shadow_ray.origin = vec_add(render->hit, vec_scale(render->normal, 0.001));
    shadow_ray.direction = render->light_dir;

    // Verifica interseção com esfera
    if (intersect_ray_sphere(shadow_ray, data->sphere, &t_sphere_shadow) &&
        t_sphere_shadow < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;

    // Verifica interseção com plano
    t_plane_shadow = intersect_ray_plane(&shadow_ray.origin, &shadow_ray.direction, data->plane);
    if (t_plane_shadow > 0 &&
        t_plane_shadow < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;

    // Verifica interseção com o próprio cilindro (ou outros, se houver)
    if (intersect_cylinder(shadow_ray, *data->cylinder, &t_cylinder_shadow) &&
        t_cylinder_shadow < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;

    // Cálculo da luz difusa
    float diffuse_intensity = in_shadow ? 0.0 : fmax(0.0, vec_dot(render->normal, render->light_dir));

    // Cor final
    render->color = ambient_light(&data->cylinder->color, diffuse_intensity, data->ambient);
}

void    sphere_shadow_check(t_render *render, t_data *data)
{
    float   t_shadow;
    double  t_plane_shadow;
    float   diffuse_intensity;
    int in_shadow;

    // Verificar se está na sombra
    in_shadow = 0;
    t_ray shadow_ray = {vec_add(render->hit, vec_scale(render->normal , 0.001)), render->light_dir}; // Pequeno offset para evitar auto-interseção

    t_plane_shadow = intersect_ray_plane(&shadow_ray.origin, &shadow_ray.direction, data->plane);
    if (intersect_ray_sphere(shadow_ray, data->sphere, &t_shadow) && t_shadow 
        < vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    else if (t_plane_shadow > 0 && t_plane_shadow < 
        vec_dot(vec_sub(data->light->position, render->hit), vec_sub(data->light->position, render->hit)))
        in_shadow = 1;
    // Calcular intensidade
    diffuse_intensity = in_shadow ? 0.0 : fmax(0.0, vec_dot(render->normal, render->light_dir));
    render->color = ambient_light(&data->sphere->color, diffuse_intensity, data->ambient);
}

// utilitário: distância ponto-a-ponto
static float vec_dist(t_vector a, t_vector b)
{
    t_vector d = { a.x-b.x, a.y-b.y, a.z-b.z };
    return sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
}

void plane_shadow_check(t_render *render, t_data *data)
{
    int   in_shadow = 0;
    float dist_light;
    float t_sph;
    double t_cyl;

    // 1) cria shadow‐ray com offset acima do plano
    t_ray shadow_ray;
    shadow_ray.origin    = vec_add(
                              render->hit,
                              vec_scale(data->plane->normalized, EPS)
                           );
    shadow_ray.direction = render->light_dir;

    // 2) distância real até a luz
    dist_light = vec_dist(data->light->position, render->hit);

    // 3) testa esfera
    if (intersect_ray_sphere(shadow_ray, data->sphere, &t_sph)
        && t_sph > EPS
        && t_sph < dist_light)
        in_shadow = 1;

    // 4) testa cilindro
    if (intersect_cylinder(shadow_ray, *data->cylinder, &t_cyl)
        && t_cyl > EPS
        && t_cyl < dist_light)
        in_shadow = 1;

    // 5) calcula componente difusa
    float diff = in_shadow
                ? 0.0f
                : fmaxf(0.0f,
                        vec_dot(data->plane->normalized,
                                render->light_dir));

    // 6) cor final: ambiente + difusa
    render->color = ambient_light(
                        &data->plane->color,
                        diff,
                        data->ambient
                    );
}

void    render_element(t_render *render, t_data *data, t_object_type type, float element)
{
    // Renderizar a objeto
    if (type == SPHERE)
    {
        render->hit = vec_add(data->ray.origin, vec_scale(data->ray.direction, element));
        render->normal = vec_normalize(vec_sub(render->hit, data->sphere->center));
        render->light_dir = vec_normalize(vec_sub(data->light->position, render->hit));
        sphere_shadow_check(data->render, data);
    }
    else if (type == PLANE)
    {
        render->hit = vec_add(data->ray.origin, vec_scale(data->ray.direction, element));
        render->light_dir = vec_normalize(vec_sub(data->light->position, render->hit));
        plane_shadow_check(data->render, data);
    }
    else if (type == CYLINDER)
    {
        render->hit = vec_add(data->ray.origin,
                    vec_scale(data->ray.direction, element));
        // determinar normal: base ou lateral?
        t_vector D = data->cylinder->normalized;
        t_vector v = vec_sub(render->hit, data->cylinder->center);
        double proj = vec_dot(v, D);
        double halfh = data->cylinder->height / 2.0;

        if (fabs(fabs(proj) - halfh) < 1e-3)
        {
            // é tampa
            render->normal = (proj > 0) ? D : vec_scale(D, -1);
        }
        else
        {
            // lateral
            t_vector proj_v = vec_scale(D, proj);
            render->normal = vec_normalize(vec_sub(v, proj_v));
        }
        cylinder_shadow_check(data->render, data);
    }
}

// intersect_ray_object para UM objeto de cada tipo
int intersect_ray_object(t_data *d)
{
    double t_min = INF;
    t_object_type hit_type;

    // 1) Esfera
    {
        float ts;
        if (intersect_ray_sphere(d->ray, d->sphere, &ts) && ts > EPS && ts < t_min)
        {
            t_min    = ts;
            hit_type = SPHERE;
        }
    }

    // 2) Plano
    {
        double tp = intersect_ray_plane(&d->ray.origin, &d->ray.direction, d->plane);
        if (tp > EPS && tp < t_min)
        {
            t_min    = tp;
            hit_type = PLANE;
        }
    }

    // 3) Cilindro
    {
        double tc;
        if (intersect_cylinder(d->ray, *d->cylinder, &tc) && tc > EPS && tc < t_min)
        {
            t_min    = tc;
            hit_type = CYLINDER;
        }
    }

    // Se houve colisão, renderiza o objeto mais próximo
    if (t_min < INF)
    {
        render_element(d->render, d, hit_type, (float)t_min);
        return 1;
    }
    return 0;
}


/*int intersect_ray_object(t_data *data)
{
    int     hit_sphere;
    int     hit_cylind;
    double  hit_plane;
    float   sphere = -1.0f;
    double  cylind = -1.0f;

    // Testa esfera
    hit_sphere = intersect_ray_sphere(data->ray, data->sphere, &sphere);

    // Testa plano
    hit_plane = intersect_ray_plane(&data->camera->origin, &data->ray.direction, data->plane);

    // Testa cilindro
    hit_cylind = intersect_cylinder(data->ray, *data->cylinder, &cylind);

    // Prioriza o objeto mais próximo
    if (hit_sphere && sphere > 0 && (hit_plane < 0 || sphere < hit_plane) && (cylind < 0 || sphere < cylind))
    {
        render_element(data->render, data, SPHERE, sphere);
        return (1);
    }
    else if (hit_plane > 0 && (cylind < 0 || hit_plane < cylind))
    {
        render_element(data->render, data, PLANE, hit_plane);
        return (1);
    }
    else if (hit_cylind && cylind > 0)
    {
        render_element(data->render, data, CYLINDER, cylind);
        return (1);
    }
    return 0;
}*/

void    render_scene(t_data *data)
{
    int xy[2];

    xy[0] = 0;
    while (xy[0]++ < 600)
    {
        xy[1] = 0;
        while(xy[1]++ < 800)
        {
            get_ray_direction(xy[1], xy[0], 800, 600, data);
            if (intersect_ray_object(data))
                put_pixel(&data->img, xy[1], xy[0], data->render->color);
            else
                put_pixel(&data->img, xy[1], xy[0], (t_color){19, 24, 33});
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

    create_data(data);
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
