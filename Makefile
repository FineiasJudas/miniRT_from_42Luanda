# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 09:47:37 by fjilaias          #+#    #+#              #
#    Updated: 2025/07/15 10:39:59 by fjilaias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT

CC = cc

CFLAGS = -Wall -Werror -Wextra  -I./minilibx-linux

RM = rm -f

SRCS =  ./src/main.c \
		./src/rt_utils/gnl.c \
		./src/rt_in/read_file.c \
		./src/rt_in/utils.c \
		./src/rt_in/parse_elements.c \
		./src/rt_utils/conv.c \
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

%.o: %.c  
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
