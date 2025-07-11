/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:58:59 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/11 16:39:30 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define WIDTH  800
# define HEIGHT 600
# define EPS 1e-4
# define INF 1e30
# define EPS 1e-4

# include "../libft/libft.h"
# include "minirt.h"
# include "mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>

//structs

typedef enum s_object_type
{
    SPHERE,
    PLANE,
    CYLINDER,
    LIGHT,
    NONE
}   t_object_type;

typedef struct s_vector
{
    double  x;
    double  y;
    double  z;
}   t_vector;

typedef struct s_color
{
    int r;
    int g;
    int b;
}   t_color;

typedef struct s_ambient
{
    double  ratio;      // valor entre 0.0 e 1.0
    t_color color;     // cor da luz ambiente
}   t_ambient;

typedef struct s_camera
{
    t_vector    origin;   // posição da câmera
    t_vector    direction; // direção normalizada (x, y, z)
    double  fov;        // campo de visão em graus
}   t_camera;

typedef struct s_light
{
    t_vector    position; // posição da luz
    double  brightness; // valor entre 0.0 e 1.0
    t_color color;     // cor da luz
    int     clicked; // 1 se a luz foi clicada, 0 caso contrário
}   t_light;

typedef struct s_sphere
{
    t_vector    center;
    double  radius;
    float   ts;
    t_color color;
    int     clicked; // 1 se a esfera foi clicada, 0 caso contrário
}   t_sphere;

typedef struct s_cylinder
{
    t_vector    center;
    t_vector    normalized;
    t_color color;
    double  diameter;
    double  height;
    double  tc;
    int     clicked; // 1 se o cilindro foi clicado, 0 caso contrário
}   t_cylinder;

typedef struct s_plane
{
    t_vector    coordinates;
    t_vector    normalized;
    t_color color;
    float   tp;
}   t_plane;

typedef struct s_image
{
    void    *img_ptr;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
    int     width;
    int     height;
}   t_image;

typedef struct s_render
{
    t_vector    hit;
    t_vector    normal;
    t_vector    light_dir;
    t_color color;
}   t_render;
     
typedef struct s_ray
{
    t_vector    origin;
    t_vector    direction;
}   t_ray;

typedef struct s_data
{
    void    *mlx;
    void    *win;
    t_image img;
    t_light *light;
    t_ambient   *ambient;
    t_camera    *camera;
    t_render    *render;
    t_vector    *cam_origin;
    t_ray   shadow_ray;
    t_ray   ray;
    t_ray   mouse_ray;

    //objectos
    t_sphere    *sphere;
    t_plane *plane;
    t_cylinder  *cylinder;
    t_list  *sphere_l;
    t_list  *lights_l;
    t_list  *cylinder_l;

    //uteis
    t_cylinder *c;
    t_sphere    *s;
    t_list  *tmp;
    t_object_type hit_type;
    
    t_list        *selected_node;
    t_object_type  selected_type;
    void          *selected_obj;

    char    *filename;
    char    *line;
    char    **tokens;
    int     fd;
    int     line_count;
    int     invalid_line;
}   t_data;

//functions
char	*get_next_line(int fd);
int     parse_rt_file(char *filename, t_data *scene);

//rt_utils/conv
t_vector conv_vector(char *str);
t_color conv_color(char *str);
void add_to_scene_list(t_list **list, void *element);
int color_to_int(t_color color);
double  float_convert(char *arg);


//rt_utils/util
int count_tokens(char **strs);

//rt_mayh
void draw_circle(t_color image[HEIGHT][WIDTH], int cx, int cy, int radius, t_color color);

//rt_init/init
void    init_image(t_image *img, void *mlx, int width, int height);
void    put_pixel(t_image *img, int x, int y, t_color color);
void    create_data(t_data *data);
void    prin_data(t_data *arg);

//src/rt_scene/vec_operations.c
t_vector vec_sub(t_vector a, t_vector b);
float vec_dot(t_vector a, t_vector b);
t_vector vec_normalize(t_vector v);
t_vector vec_scale(t_vector v, float s);
t_vector vec_add(t_vector a, t_vector b);
t_color scale_color(t_color color, float factor);

int intersect_ray_cylinder(t_ray ray, t_cylinder *cy, float *t_out, int *is_cap);
t_vector get_cylinder_normal(t_vector hit, t_cylinder *cy, int is_cap);

//src/rt_scene/utils.c
t_color ambient_light(t_color *src, double intensity, t_ambient *ambient);
int get_ray_direction(int x, int y, int width, int height, t_data *data);
int get_mouse_direction(int x, int y, int width, int height, t_data *data);
t_color calc_ambient(t_color *c, t_ambient *ambient);

//src/rt_render
int    render_element(t_render *render, t_data *data, t_object_type type, float element);
int intersect_ray_object(t_data *d);
void    render_scene(t_data *data);

//src/rt_scene
double intersect_ray_plane(t_vector *ray_origin, t_vector *ray_dir, t_plane *plane);
int shadow_plane_check(t_render *render, t_data *data);

int intersect_ray_sphere(t_ray ray, t_sphere *s, float *t);
int    sphere_shadow_check(t_render *render, t_data *data);

//src/rt_scene/cylinder.c
int intersect_cylinder(t_ray ray, t_cylinder cyl, double *t_out);
int cylinder_shadow_check(t_render *render, t_data *data);

//src/rt_scene/shadow_checkers.c
int shadow_spheres_check(t_render *render, t_data *data);
//int shadow_plane_check(t_render *render, t_data *data);
int shadow_cylinders_check(t_render *render, t_data *data);

//src/rt_in/parse_elements.c
int parse_plane(char **tokens, t_data *plane);
int parse_sphere(char **tokens, t_data *scene);
int parse_ambient(char **tokens, t_data *a);
int parse_cylinder(char **tokens, t_data *cylinder);
int parse_light(char **tokens, t_data *light);
int parse_camera(char **tokens, t_data *scene);

//src/rt_in/utils.c
int is_first_word_one_of(char *line);
int open_rt_file(char *filename);
char    *ft_strtok(char *str, const char *delim);
int file_line_counter(int fd, t_data *data);

// src/rt_in/read_file.c
int ft_strcmp(const char *s1, const char *s2);
int identify_and_process(char **tokens, t_data *data);
int process_line(char *line, t_data *data);
int read_and_process_lines(int fd, t_data * data);
int parse_rt_file(char *filename, t_data *data);

//src/hooks/hooks.c
int close_window(t_data *data);
int resize_window(int width, int height, t_data *data);
int mouse_move(int x, int y, t_data *data);
int mouse_click(int button, int x, int y, t_data *data);
int camera_move(int keycode, t_data *data);
int key_press(int keycode, t_data *data);

// src/rt_free_up/free_up.c
void *gc_malloc(t_list **garbage, size_t size);
void free_all(t_list **garbage);
int free_tokens(char **tokens);
void free_list(t_list **list);

void my_free(void *ptr);
void *my_malloc(size_t size);
void* mm_alloc(size_t size);
void mm_free(void *ptr);
void mm_free_all(void);
void mm_init(void);

void	collect_mem(void *content);
void	*allocate_mem(size_t nmemb, size_t size);
void	collect_env_mem(char **env_table);
void	free_mem(t_list **list);
t_list	**get_mem_address(void);

#endif
