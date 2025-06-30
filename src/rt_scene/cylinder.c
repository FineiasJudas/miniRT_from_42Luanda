/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:03 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 08:46:25 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

/*float get_min_t(float t1, float t2, float t_base, float t_top)
{
    float t_min = -1.0f;
    if (t1 > 0 && (t_min < 0 || t1 < t_min))
        t_min = t1;
    if (t2 > 0 && (t_min < 0 || t2 < t_min))
        t_min = t2;
    if (t_base > 0 && (t_min < 0 || t_base < t_min))
        t_min = t_base;
    if (t_top > 0 && (t_min < 0 || t_top < t_min))
        t_min = t_top;
    return t_min;
}*/