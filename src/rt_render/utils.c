/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:37:06 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/26 01:45:35 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	check_spheres(t_data *d, double *t_min)
{
	d->tmp = d->sphere_l;
	while (d->tmp)
	{
		d->s = (t_sphere *)d->tmp->content;
		if (intersect_ray_sphere(d->ray, d->s, &d->s->ts) && d->s->ts > EPS
			&& d->s->ts < *t_min)
		{
			*t_min = d->s->ts;
			d->hit_type = SPHERE;
			d->sphere = d->s;
		}
		d->tmp = d->tmp->next;
	}
	return (0);
}

static int	check_planes(t_data *d, double *t_min)
{
	d->tmp = d->plane_l;
	while (d->tmp)
	{
		d->p = (t_plane *)d->tmp->content;
		d->p->tp = intersect_ray_plane(&d->ray.origin, &d->ray.direction, d->p);
		if (intersect_ray_plane(&d->ray.origin, &d->ray.direction, d->p)
			&& d->p->tp > EPS && d->p->tp < *t_min)
		{
			*t_min = d->p->tp;
			d->hit_type = PLANE;
			d->plane = d->p;
		}
		d->tmp = d->tmp->next;
	}
	return (0);
}

static int	check_cylinders(t_data *d, double *t_min)
{
	d->tmp = d->cylinder_l;
	while (d->tmp)
	{
		d->c = (t_cylinder *)d->tmp->content;
		if (intersect_cylinder(d->ray, *d->c, &d->c->tc) && d->c->tc > EPS
			&& d->c->tc < *t_min)
		{
			*t_min = d->c->tc;
			d->hit_type = CYLINDER;
			d->cylinder = d->c;
		}
		d->tmp = d->tmp->next;
	}
	return (0);
}

static int	check_light_as_sphere(t_data *d, double *t_min)
{
	if (intersect_ray_sphere(d->ray, d->light_s, &d->light_s->ts)
		&& d->light_s->ts < *t_min && d->light_s->ts > EPS)
	{
		*t_min = d->light_s->ts;
		d->hit_type = LIGHT;
	}
	d->render->color = scale_color(d->light_s->color, d->light->brightness);
	return (0);
}

int	intersect_ray_object(t_data *d)
{
	double	t_min;

	t_min = INF;
	check_spheres(d, &t_min);
	check_planes(d, &t_min);
	check_cylinders(d, &t_min);
	check_light_as_sphere(d, &t_min);
	if (t_min < INF)
		return (render_element(d->render, d, d->hit_type, (float)t_min));
	return (0);
}
