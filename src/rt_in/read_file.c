#include "minirt.h"

/**
 * Compara duas strings
 * @param s1 Primeira string
 * @param s2 Segunda string
 * @return Diferença entre os caracteres diferentes ou 0 se iguais
 */
int ft_strcmp(const char *s1, const char *s2)
{
    unsigned int i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int parse_plane(char **tokens)
{
    // Verificar se tem tokens suficientes
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (1);
    // Implementar a lógica de
    return (0);
}

int parse_sphere(char **tokens, t_scene *scene)
{
    // Verificar se tem tokens suficientes
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (1);

    // Implementar a lógica de
    if (count_tokens(tokens) != 4)
        printf("Formato inválido para esfera");

    t_sphere *sp = malloc(sizeof(t_sphere));
    sp->center = conv_vector(tokens[1]);
    sp->radius = ft_atoi(tokens[2]) / 2.0;
    sp->color = conv_color(tokens[3]);

    add_to_scene_list(&scene->spheres, sp);
    return (0);
}

int parse_ambient(char **tokens)
{
    // Verificar se tem tokens suficientes
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (1);
    // Implementar a lógica de
    return (0);
}

int parse_cylinder(char **tokens)
{
    // Verificar se tem tokens suficientes
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (1);
    // Implementar a lógica de
    return (0);
}
int parse_light(char **tokens)
{
    // Verificar se tem tokens suficientes
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (1);
    // Implementar a lógica de
    return (0);
}

int parse_camera(char **tokens)
{
    // Verificar se tem tokens suficientes
    if (!tokens[1] || !tokens[2] || !tokens[3])
        return (1);
    // Implementar a lógica de
    return (0);
}

int identify_and_process(char **tokens, t_scene *scene)
{
    if (!tokens || !tokens[0])
        return (1);


    printf("::::::::::::: type:: %s\n", tokens[0]);
    if (ft_strcmp(tokens[0], "A") == 0)
        parse_ambient(tokens);
    else if (ft_strcmp(tokens[0], "C") == 0)
        parse_camera(tokens);
    else if (ft_strcmp(tokens[0], "L") == 0)
        parse_light(tokens);
    else if (ft_strcmp(tokens[0], "sp") == 0)
        parse_sphere(tokens, scene);
    else if (ft_strcmp(tokens[0], "pl") == 0)
        parse_plane(tokens);
    else if (ft_strcmp(tokens[0], "cy") == 0)
        parse_cylinder(tokens);
    else
        printf("Identificador inválido");

    // Adicione outros tipos conforme necessário

    return (1); // Tipo desconhecido
}

int process_line(char *line, t_scene *scene)
{
    char **tokens;
    int result;

    tokens = ft_split(line, ' ');
    if (!tokens)
        return (1);

    result = identify_and_process(tokens, scene);

    // free_tokens(tokens);
    return (result);
}

int read_and_process_lines(int fd, t_scene * scene)
{
    char *line;
    int result;

    while (1)
    {
        line = get_next_line(fd);
        if (!line)
            break;

        result = process_line(line, scene);
        free(line);

        if (result)
            return (result);
    }

    return (0);
}

int open_rt_file(char *filename)
{
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);
    return (fd);
}

int parse_rt_file(char *filename, t_scene *scene)
{
    int fd;
    int result;
    (void)filename;

    fd = open_rt_file("models/test.rt");
    if (fd < 0)
        return (1);
    result = read_and_process_lines(fd, scene);
    close(fd);
    return (result);
}