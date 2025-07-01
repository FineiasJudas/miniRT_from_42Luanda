/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:03 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/01 13:49:26 by fjilaias         ###   ########.fr       */
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
    float diffuse_intensity;
    int in_shadow;

    in_shadow = 0;
    if(shadow_spheres_check(render, data))
        in_shadow = 1;
    // Verifica interseção com plano
    if (shadow_plane_check(render, data))
        in_shadow = 1;
    // Verifica interseção com o próprio cilindro (ou outros, se houver)
    if (shadow_cylinders_check(render, data))
        in_shadow = 1;
    // Cálculo da luz difusa
    if (in_shadow)
        diffuse_intensity = 0.0;
    else
        diffuse_intensity = fmax(0.0, vec_dot(render->normal, render->light_dir));
    // Cor final
    render->color = ambient_light(&data->cylinder->color, diffuse_intensity, data->ambient);
}
