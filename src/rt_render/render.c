/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:47:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/30 14:14:15 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void    cylinder_normal(t_render *render, t_data *data, float element)
{
    t_vector D;
    t_vector v;
    t_vector proj_v;
    double proj;
    double halfh;

    render->hit = vec_add(data->ray.origin,
                    vec_scale(data->ray.direction, element));
    // determinar normal: base ou lateral?
    D = data->cylinder->normalized;
    v = vec_sub(render->hit, data->cylinder->center);
    proj = vec_dot(v, D);
    halfh = data->cylinder->height / 2.0;
    if (fabs(fabs(proj) - halfh) < 1e-3)
    {   // é tampa
        if ((proj > 0))
            render->normal = D;
        else
            render->normal = vec_scale(D, -1);
    }
    else
    {   // lateral
        proj_v = vec_scale(D, proj);
        render->normal = vec_normalize(vec_sub(v, proj_v));
    }
}

int    render_element(t_render *render, t_data *data, t_object_type type, float element)
{
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
        cylinder_normal(data->render, data, element);
        cylinder_shadow_check(data->render, data);
    }
    return (1);
}

int intersect_ray_object(t_data *d)
{
    double        t_min = INF;
    t_object_type hit_type;

    t_list *node = d->sphere_l;
    while(node)
    {
        t_sphere *s = (t_sphere*)node->content;
        if (intersect_ray_sphere(d->ray, s, &s->ts)
            && s->ts > EPS && s->ts < t_min)
        {
            t_min       = s->ts;
            hit_type    = SPHERE;
            d->sphere   = s;      // marca qual esfera colidiu
        }
        node = node->next;
    }

    // 2) PLANO (único)
    {
        double tp = intersect_ray_plane(
                        &d->ray.origin,
                        &d->ray.direction,
                        d->plane);
        if (tp > EPS && tp < t_min)
        {
            t_min     = tp;
            hit_type  = PLANE;
        }
    }

    // 3) CILINDRO (único)
    {
        double tc;
        if (intersect_cylinder(d->ray, *d->cylinder, &tc)
            && tc > EPS
            && tc < t_min)
        {
            t_min        = tc;
            hit_type     = CYLINDER;
        }
    }
    if (t_min < INF)
    {
        render_element(d->render, d, hit_type, (float)t_min);
        return 1;
    }
    return 0;
}


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
