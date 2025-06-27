/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:03 by fjilaias          #+#    #+#             */
/*   Updated: 2025/06/26 15:28:08 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int intersect_cylinder_body(t_ray ray, t_cylinder *cy, float *t1, float *t2)
{
    t_vector O = ray.origin;         // Origem do raio
    t_vector D = ray.direction;      // Direção do raio (normalizada)
    t_vector C = cy->center;         // Centro da base do cilindro
    t_vector A = cy->normalized;     // Eixo do cilindro
    double R = cy->diameter / 2.0;   // Raio do cilindro
    double H = cy->height;           // Altura do cilindro

    // Vetor do centro à origem, projetado no eixo e perpendicular
    t_vector OC = vec_sub(O, C);
    double OC_proj = vec_dot(OC, A);
    double D_proj = vec_dot(D, A);
    t_vector OC_perp = vec_sub(OC, vec_scale(A, OC_proj));
    t_vector D_perp = vec_sub(D, vec_scale(A, D_proj));

    // Coeficientes da equação quadrática: a*t² + b*t + c = 0
    double a = vec_dot(D_perp, D_perp);
    double b = 2 * vec_dot(OC_perp, D_perp);
    double c = vec_dot(OC_perp, OC_perp) - R * R;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return 0; // Sem interseção

    // Calcular t1 e t2
    *t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    *t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    // Verificar se t1 está dentro da altura
    int valid_t1 = 0, valid_t2 = 0;
    if (*t1 > 0)
    {
        t_vector P1 = vec_add(O, vec_scale(D, *t1));
        double P1_proj = vec_dot(vec_sub(P1, C), A);
        if (P1_proj >= 0 && P1_proj <= H)
            valid_t1 = 1;
    }
    if (*t2 > 0)
    {
        t_vector P2 = vec_add(O, vec_scale(D, *t2));
        double P2_proj = vec_dot(vec_sub(P2, C), A);
        if (P2_proj >= 0 && P2_proj <= H)
            valid_t2 = 1;
    }

    // Ajustar t1 e t2 para valores inválidos
    if (!valid_t1)
        *t1 = -1.0f;
    if (!valid_t2)
        *t2 = -1.0f;

    return (valid_t1 || valid_t2); // Retorna 1 se pelo menos uma interseção é válida
}

int intersect_cylinder_caps(t_ray ray, t_cylinder *cy, float *t_base, float *t_top)
{
    t_vector O = ray.origin;
    t_vector D = ray.direction;
    t_vector C = cy->center;
    t_vector A = cy->normalized;
    double R = cy->diameter / 2.0;

    *t_base = -1.0f;
    *t_top = -1.0f;

    // Interseção com a tampa inferior (base)
    double denom = vec_dot(D, A);
    if (fabs(denom) > 1e-6) // Evita divisão por zero
    {
        double t = vec_dot(vec_sub(C, O), A) / denom;
        if (t > 0)
        {
            t_vector P = vec_add(O, vec_scale(D, t));
            t_vector diff = vec_sub(P, C);
            if (vec_dot(diff, diff) <= R)
                *t_base = t;
        }

        // Interseção com a tampa superior (topo)
        t_vector C_top = vec_add(C, vec_scale(A, cy->height));
        t = vec_dot(vec_sub(C_top, O), A) / denom;
        if (t > 0)
        {
            t_vector P = vec_add(O, vec_scale(D, t));
            t_vector diff = vec_sub(P, C_top);
            if (vec_dot(diff, diff) <= R)
                *t_top = t;
        }
    }

    return (*t_base > 0 || *t_top > 0); // Retorna 1 se pelo menos uma tampa foi atingida
}

float get_min_t(float t1, float t2, float t_base, float t_top)
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
}

int intersect_ray_cylinder(t_ray ray, t_cylinder *cy, float *t_out, int *is_cap)
{
    float t1, t2, t_base, t_top;
    *is_cap = 0; // Indica se a interseção foi com uma tampa

    // Verificar interseções com corpo e tampas
    int hit_body = intersect_cylinder_body(ray, cy, &t1, &t2);
    int hit_caps = intersect_cylinder_caps(ray, cy, &t_base, &t_top);

    (void)hit_body;
    (void)hit_caps;
    // Encontrar menor t válido
    float t_min = get_min_t(t1, t2, t_base, t_top);
    if (t_min <= 0)
        return 0; // Sem interseção válida

    // Verificar se a interseção é com uma tampa
    if (t_min == t_base || t_min == t_top)
        *is_cap = 1;

    *t_out = t_min;
    return 1; // Interseção encontrada
}

t_vector get_cylinder_normal(t_vector hit, t_cylinder *cy, int is_cap)
{
    t_vector normal;
    if (is_cap)
    {
        // Normal da tampa é o eixo ou seu oposto
        t_vector C_top = vec_add(cy->center, vec_scale(cy->normalized, cy->height));
        double dist_to_base = vec_dot(vec_sub(hit, cy->center), vec_sub(hit, cy->center));
        double dist_to_top = vec_dot(vec_sub(hit, C_top), vec_sub(hit, C_top));
        normal = (dist_to_base < dist_to_top) ? cy->normalized : vec_scale(cy->normalized, -1.0);
    }
    else
    {
        // Normal do corpo: projeção perpendicular ao eixo
        t_vector CH = vec_sub(hit, cy->center);
        double CH_proj = vec_dot(CH, cy->normalized);
        t_vector proj = vec_scale(cy->normalized, CH_proj);
        normal = vec_normalize(vec_sub(CH, proj));
    }
    return normal;
}