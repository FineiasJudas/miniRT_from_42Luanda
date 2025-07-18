/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:58:59 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/18 15:44:41 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# ifndef PI
#  define PI 3.14159265358979323846
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define WIDTH 800
# define HEIGHT 600
# define EPS 1e-4
# define INF 1e30	

# include "../libft/libft.h"
# include "minirt.h"
# include "mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>

typedef enum s_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	LIGHT,
	NONE
}					t_object_type;

typedef struct s_vector
{
	double			x;
	double			y;
	double			z;
}					t_vector;

typedef struct s_rotate_v
{
	t_vector		r;
	double			cos_a;
	double			sin_a;
}	t_rotate_v;


typedef struct s_cyl_vars
{
	double			r;
	double			halfh;
	t_vector		d;
	t_vector		co;
	t_vector		v;
	t_vector		m;
	t_vector		n;
	double			a;
	double			b;
	double			c;
	double			disc;
	double			t0;
	double			t1;
	double			t_lat;
	double			t_min;
	int				hit;
}					t_cyl_vars;

typedef struct s_color
{
	int				r;
	int				g;
	int				b;
}					t_color;

typedef struct s_ambient
{
	double			ratio;
	t_color			color;
}					t_ambient;

typedef struct s_camera
{
	t_vector		origin;
	t_vector		dir;
	double			fov;
}					t_camera;

typedef struct s_light
{
	t_vector		position;
	double			brightness;
	t_color			color;
}					t_light;

typedef struct s_sphere
{
	t_vector		center;
	double			radius;
	float			ts;
	t_color			color;
}					t_sphere;

typedef struct s_cylinder
{
	t_vector		center;
	t_vector		normalized;
	t_color			color;
	double			diameter;
	double			height;
	double			tc;
}					t_cylinder;

typedef struct s_plane
{
	t_vector		coordinates;
	t_vector		normalized;
	t_color			color;
	float			tp;
}					t_plane;

typedef struct s_image
{
	void			*img_ptr;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				width;
	int				height;
}					t_image;

typedef struct s_render
{
	t_vector		hit;
	t_vector		normal;
	t_vector		light_dir;
	t_color			color;
}					t_render;

typedef struct s_ray
{
	t_vector		origin;
	t_vector		direction;
}					t_ray;

typedef struct s_data
{
	void			*mlx;
	void			*win;
	t_image			img;
	t_ambient		*ambient;
	t_camera		*camera;
	t_render		*render;
	t_vector		*cam_origin;
	t_ray			shadow_ray;
	t_ray			ray;

	t_plane			*plane;
	t_sphere		*sphere;
	t_sphere		*light_s;
	t_light			*light;
	t_cylinder		*cylinder;
	t_list			*plane_l;
	t_list			*sphere_l;
	t_list			*lights_l;
	t_list			*cylinder_l;

	t_cylinder		*c;
	t_sphere		*s;
	t_plane			*p;
	t_light			*l;
	t_list			*tmp;
	t_object_type	hit_type;

	t_list			*selected_node;
	t_object_type	selected_type;
	void			*selected_obj;

	t_rotate_v		rot;

	char			*filename;
	char			*line;
	char			**tokens;
	int				fd;
	int				invalid_line;
	char			**matrix;
}					t_data;

char				*get_next_line(int fd);
int					parse_rt_file(char *filename, t_data *scene);
t_vector			conv_vector(char *str);
t_color				conv_color(char *str);
void				add_to_scene_list(t_list **list, void *element);
int					color_to_int(t_color color);
double				float_convert(char *arg);
int					count_tokens(char **strs);
void				init_image(t_image *img, void *mlx, int width, int height);
void				put_pixel(t_image *img, int x, int y, t_color color);
void				create_data(t_data *data);
void				prin_data(t_data *arg);
t_vector			cross(t_vector a, t_vector b);
t_vector			vec_sub(t_vector a, t_vector b);
float				vec_dot(t_vector a, t_vector b);
t_vector			vec_normalize(t_vector v);
t_vector			vec_scale(t_vector v, float s);
t_vector			vec_add(t_vector a, t_vector b);
t_color				scale_color(t_color color, float factor);
void				cylinder_normal(t_render *render, t_data *data, float t);
t_color				ambient_light(t_color *src, double intensity,
						t_ambient *ambient);
int					get_ray_direction(int *xy, int width, int height,
						t_data *data);
t_color				calc_ambient(t_color *c, t_ambient *ambient);
int					render_element(t_render *render, t_data *data,
						t_object_type type, float element);
int					intersect_ray_object(t_data *d);
void				render_scene(t_data *data);
double				intersect_ray_plane(t_vector *ray_origin, t_vector *ray_dir,
						t_plane *plane);
int					shadow_planes_check(t_render *render, t_data *data);
int					plane_shadow_check(t_render *render, t_data *data);
int					intersect_ray_sphere(t_ray ray, t_sphere *s, float *t);
int					sphere_shadow_check(t_render *render, t_data *data);
int					intersect_cylinder(t_ray ray, t_cylinder cyl,
						double *t_out);
int					cylinder_shadow_check(t_render *render, t_data *data);
int					shadow_spheres_check(t_render *render, t_data *data);
int					shadow_cylinders_check(t_render *render, t_data *data);
int					parse_plane(char **tokens, t_data *plane);
int					parse_sphere(char **tokens, t_data *scene);
int					parse_ambient(char **tokens, t_data *a);
int					parse_cylinder(char **tokens, t_data *cylinder);
int					parse_light(char **tokens, t_data *light);
int					parse_camera(char **tokens, t_data *scene);
float				vec_dist(t_vector a, t_vector b);
int					is_first_word_one_of(char *line);
int					open_rt_file(char *filename);
char				*ft_strtok(char *str, const char *delim);
int					file_line_counter(int fd, t_data *data);
int					ft_strcmp(const char *s1, const char *s2);
int					identify_and_process(char **tokens, t_data *data);
int					process_line(char *line, t_data *data);
int					read_and_process_lines(t_data *data);
int					close_window(t_data *data);
int					resize_window(int width, int height, t_data *data);
int					camera_move(int keycode, t_data *data);
int					key_press(int keycode, t_data *data);
void				*gc_malloc(t_list **garbage, size_t size);
void				free_all(t_list **garbage);
int					free_tokens(char **tokens);
void				free_list(t_list **list);
void				collect_mem(void *content);
void				*allocate_mem(size_t nmemb, size_t size);
void				collect_env_mem(char **env_table);
void				free_mem(t_list **list);
t_list				**get_mem_address(void);
t_vector			look_at(t_vector from, t_vector to);
t_vector			rotate_vector(t_data *data, t_vector v, char axis, double angle);
int					is_blank_line(const char *s);
int					plane_move(int keycode, t_plane *p, t_data *data);
int					sphere_move(int keycode, t_sphere *s, t_data *data);
void				cylinder_move(int keycode, t_cylinder *c, t_data *data);
int					light_move(int keycode, t_light *light, t_data *data);
int					camera_move(int keycode, t_data *data);
int					any_object_moviment_key(int keycode);
int					any_camera_moviment_key(int keycode);
void				print_log(t_object_type type, t_data *data);
void				gnl_free_stash(void);
char				*m_strjoin(char *s1, char *s2);
char				*m_strchr(char *s, int c);
size_t				m_strlen(char *str);
void				object_log(t_data *data);
char				**read_file_into_matrix(int fd, int *out_count, t_data *data);
char				*validate_ambient(char *str);
char *trim(char *s);
bool is_valid_double(const char* str);
char* removeEspacosETabs(char *str);
bool is_valid_vector3d(const char* str);
char *validate_camera(char *str);
bool is_valid_vector3d(const char* str);
char *validate_light(char *str);
bool is_valid_rgb(const char* str);
char *validate_plane(char *str);


#endif
