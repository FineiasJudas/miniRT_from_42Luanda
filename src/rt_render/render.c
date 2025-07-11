/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:47:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/10 10:37:35 by fjilaias         ###   ########.fr       */
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
        shadow_plane_check(data->render, data);
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
    
    d->tmp = d->sphere_l;
    while(d->tmp)
    {
        d->s = (t_sphere*)d->tmp->content;
        if (intersect_ray_sphere(d->ray, d->s, &d->s->ts) && d->s->ts > EPS && d->s->ts < t_min)
        {
            t_min        = d->s->ts;
            d->hit_type    = SPHERE;
            d->sphere   = d->s;
        }
        d->tmp = d->tmp->next;
    }
    d->plane->tp = intersect_ray_plane( &d->ray.origin, &d->ray.direction, d->plane);
    if (d->plane->tp > EPS && d->plane->tp < t_min)
    {
        t_min     = d->plane->tp;
        d->hit_type  = PLANE;
    }
    d->tmp = d->cylinder_l;
    while (d->tmp)
    {
        d->c = (t_cylinder *)d->tmp->content;
        if (intersect_cylinder(d->ray, *d->c, &d->c->tc) && d->c->tc > EPS && d->c->tc < t_min)
        {
            t_min        = d->c->tc;
            d->hit_type     = CYLINDER;
            d->cylinder   = d->c;
        }
        d->tmp = d->tmp->next;
    }
    if (t_min < INF)
        return (1 * render_element(d->render, d, d->hit_type, (float)t_min)); 
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
