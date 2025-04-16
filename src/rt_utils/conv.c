#include "minirt.h"

t_vector conv_vector(char *str)
{
    char **components = ft_split(str, ',');
    t_vector v;

    if (!components || count_tokens(components) != 3)
        printf("Coordenadas de vetor inválidas");

    v.x = ft_atoi(components[0]);
    v.y = ft_atoi(components[1]);
    v.z = ft_atoi(components[2]);

    //free_split(components); // liberta memória (usa tua função)
    return v;
}

t_color conv_color(char *str)
{
    char **components = ft_split(str, ',');
    t_color c;

    if (!components || count_tokens(components) != 3)
        printf("Coordenadas de vetor inválidas");

    c.r = ft_atoi(components[0]);
    c.g = ft_atoi(components[1]);
    c.b = ft_atoi(components[2]);

    //free_split(components); // liberta memória (usa tua função)
    return c;
}



void add_to_scene_list(t_list **list, void *element)
{
    t_list *node;

    if (!list || !element)
        return;

    node = ft_lstnew(element);
    if (!node)
        printf("Erro ao alocar memória para objeto");

    ft_lstadd_back(list, node);
}

