#include "minirt.h"

double count_digits(char *arg)
{
    char    *base;
    char    *out;
    double  number;

    if (!arg)
        return (-1);
    base = malloc(sizeof(char) * 1);
    if (!base)
        printf("Malloc não funconou!\n");
    ft_strlcpy(base, "1", 2);
    out = ft_strjoin(base, arg);
    number = ft_atoi(out) - ft_atoi(arg);
    return (number);
}

double  float_convert(char *arg)
{
    char *integer;
    char *floater;
    double in;
    double f;
    double out;

    if (!arg || ft_strlen(arg) < 1)
        return (-1);
    floater = ft_strchr(arg, '.');
    if (floater)
    {
        *floater = '\0';
        floater ++;
        integer = arg;
    }
    else
    {
        printf("%s não é um tipo ponto flutuante\n", arg);
        return (-1);
    }
    in = ft_atoi(integer);
    f = ft_atoi(floater);
    out = in + (f / count_digits(floater));
    printf("Final - %4f\n", out);
    return (out);
}

t_vector    conv_vector(char *str)
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
        return ;

    node = ft_lstnew(element);
    if (!node)
    {
        fprintf(stderr, "Erro ao alocar memória para objeto\n");
        exit(1);
    }
    ft_lstadd_back(list, node);
}

int color_to_int(t_color color)
{
    return ((color.r & 0xFF) << 16) | ((color.g & 0xFF) << 8) | (color.b & 0xFF);
}
