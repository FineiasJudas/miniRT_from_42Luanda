/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:26:54 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/21 09:35:11 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_cyl_vars(t_cyl_vars *v, t_ray ray, t_cylinder cyl)
{
	v->r = cyl.diameter * 0.5;
	v->halfh = cyl.height * 0.5;
	v->d = cyl.normalized;
	v->co = vec_sub(ray.origin, cyl.center);
	v->v = ray.direction;
	v->m = vec_sub(v->v, vec_scale(v->d, vec_dot(v->v, v->d)));
	v->n = vec_sub(v->co, vec_scale(v->d, vec_dot(v->co, v->d)));
}

static int	solve_quadratic(t_cyl_vars *v)
{
	double	sd;

	v->a = vec_dot(v->m, v->m);
	v->b = 2.0 * vec_dot(v->m, v->n);
	v->c = vec_dot(v->n, v->n) - v->r * v->r;
	v->disc = v->b * v->b - 4 * v->a * v->c;
	if (v->disc < 0.0 || fabs(v->a) < EPS)
		return (0);
	sd = sqrt(v->disc);
	v->t0 = (-v->b - sd) / (2.0 * v->a);
	v->t1 = (-v->b + sd) / (2.0 * v->a);
	if (v->t0 > EPS)
		v->t_lat = v->t0;
	else
		v->t_lat = INF;
	if (v->t1 > EPS && v->t1 < v->t_lat)
		v->t_lat = v->t1;
	return (1);
}

static void	check_lateral(t_cyl_vars *v, t_ray ray, t_cylinder cyl)
{
	t_vector	p;
	double		y;

	if (v->t_lat < INF)
	{
		p = vec_add(ray.origin, vec_scale(ray.direction, v->t_lat));
		y = vec_dot(v->d, vec_sub(p, cyl.center));
		if (y >= -v->halfh && y <= v->halfh)
		{
			v->t_min = v->t_lat;
			v->hit = 1;
		}
	}
}

static void	check_caps(t_cyl_vars *v, t_ray ray, t_cylinder cyl)
{
	double		denom;
	t_vector	cc;
	double		tc;
	t_vector	pcap;
	int			s;

	denom = vec_dot(ray.direction, v->d);
	if (fabs(denom) < EPS)
		return ;
	s = -1;
	while (s <= 1)
	{
		cc = vec_add(cyl.center, vec_scale(v->d, s * v->halfh));
		tc = vec_dot(vec_sub(cc, ray.origin), v->d) / denom;
		if (tc > EPS && tc < v->t_min)
		{
			pcap = vec_add(ray.origin, vec_scale(ray.direction, tc));
			if (vec_dot(vec_sub(pcap, cc), vec_sub(pcap, cc)) <= v->r * v->r)
			{
				v->t_min = tc;
				v->hit = 1;
			}
		}
		s += 2;
	}
}

int	intersect_cylinder(t_ray ray, t_cylinder cyl, double *t_out)
{
	t_cyl_vars	v;

	ft_bzero(&v, 0);
	init_cyl_vars(&v, ray, cyl);
	if (!solve_quadratic(&v))
		return (0);
	v.hit = 0;
	v.t_min = INF;
	check_lateral(&v, ray, cyl);
	check_caps(&v, ray, cyl);
	if (!v.hit)
		return (0);
	*t_out = v.t_min;
	return (1);
}
