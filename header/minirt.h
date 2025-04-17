/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:58:59 by fjilaias          #+#    #+#             */
/*   Updated: 2025/02/11 10:00:28 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

#define WIDTH  800
#define HEIGHT 600

#include "../libft/libft.h"
#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>

//structs
typedef struct s_vector {
    double x, y, z;
} t_vector;

typedef struct s_color {
    int r, g, b;
} t_color;

typedef struct s_ambient {
    double ratio;      // valor entre 0.0 e 1.0
    t_color color;     // cor da luz ambiente
} t_ambient;
typedef struct s_camera {
    t_vector origin;   // posição da câmera
    t_vector direction; // direção normalizada (x, y, z)
    double fov;        // campo de visão em graus
} t_camera;
typedef struct s_light {
    t_vector position; // posição da luz
    double brightness; // valor entre 0.0 e 1.0
    t_color color;     // cor da luz
} t_light;
typedef struct s_sphere {
    t_vector center;
    double radius;
    t_color color;
} t_sphere;
typedef struct s_scene {
    t_ambient ambient;
    t_camera camera;
    t_list *lights;     // lista de t_light
    t_list *spheres;    // lista de t_sphere
    // futuramente: planos, cilindros...
} t_scene;

typedef struct s_image
{
    void    *img_ptr;     // ponteiro para a imagem criada com mlx_new_image
    char    *addr;        // ponteiro para os dados da imagem (pixel buffer)
    int     bits_per_pixel;
    int     line_length;
    int     endian;
    int     width;
    int     height;
}   t_image;

typedef struct s_ray
{
    t_vector origin;     // ponto de origem do raio (da câmera, por exemplo)
    t_vector direction;  // direção do raio (normalizada)
} t_ray;


//functions
char	*get_next_line(int fd);
int parse_rt_file(char *filename, t_scene *scene);

//rt_utils/conv
t_vector conv_vector(char *str);
t_color conv_color(char *str);
void add_to_scene_list(t_list **list, void *element);
int color_to_int(t_color color);

//rt_utils/util
int count_tokens(char **strs);

//rt_mayh
void draw_circle(t_color image[HEIGHT][WIDTH], int cx, int cy, int radius, t_color color);

//rt_init/init
t_scene *create_scene(void);
void init_image(t_image *img, void *mlx, int width, int height);
void put_pixel(t_image *img, int x, int y, t_color color);

//

#endif
