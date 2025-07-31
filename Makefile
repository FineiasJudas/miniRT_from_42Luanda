# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/15 16:47:54 by fjilaias          #+#    #+#              #
#    Updated: 2025/07/31 12:28:57 by fjilaias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT

CC = cc

CFLAGS = -Wall -Werror -Wextra -I./header \
         -I./libft \
         -I./minilibx-linux \

SRCS =  ./src/main.c \
		./src/rt_utils/gnl.c \
		./src/rt_utils/gnl_utils.c \
		./src/rt_in/read_file.c \
		./src/rt_in/utils.c \
		./src/rt_in/utils_2.c \
		./src/rt_in/validate.c \
		./src/rt_in/validate_camera.c \
		./src/rt_in/validade_diff_ligth.c \
		./src/rt_in/validate_ambient.c \
		./src/rt_in/parse_elements.c \
		./src/rt_in/validade_plane.c  \
		./src/rt_in/validade_cylind.c \
		./src/rt_in/validate_sphere.c \
		./src/rt_utils/conv.c \
		./src/rt_utils/conv_2.c \
		./src/rt_utils/utils.c \
		./src/rt_utils/vec_operations.c \
		./src/rt_init/init.c \
		./src/rt_scene/cylinder.c \
		./src/rt_scene/plane.c \
		./src/rt_scene/sphere.c \
		./src/rt_scene/shadow_checkers.c \
		./src/rt_scene/utils.c \
		./src/rt_render/render.c \
		./src/rt_render/utils.c \
		./src/rt_hooks/hooks.c \
		./src/rt_hooks/utils_2.c \
		./src/rt_free_up/free_up.c \
		./src/rt_free_up/allocate_mem.c \
		./src/rt_free_up/free_mem.c \
		./src/rt_hooks/utils.c \
		./src/rt_hooks/hooks_2.c \

MINILIBX = -L ./minilibx-linux -lmlx -lXext -lX11 -lm

LIBPF = -L ./libft -lft

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C ./libft
	$(MAKE) bonus -C ./libft
	$(MAKE) -C ./minilibx-linux
	$(CC) $(OBJS) -o $(NAME) $(LIBPF) $(MINILIBX)

%.o: %.c header/minirt.h
	$(CC) $(CFLAGS) -Iheader -c $< -o $@

clean:
	$(MAKE) -C ./libft clean
	$(RM) $(OBJS)  

fclean: clean  
	$(RM) $(NAME)
	$(MAKE) -C ./libft fclean
	$(MAKE) -C ./minilibx-linux clean
  
re: fclean all

.PHONY: all clean fclean re