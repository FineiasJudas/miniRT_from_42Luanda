# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 09:47:37 by fjilaias          #+#    #+#              #
#    Updated: 2025/02/11 10:20:36 by fjilaias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT  

CC = cc  

CFLAGS = -Wall -Werror -Wextra  

RM = rm -f

SRCS = ./src/main.c

MINILIBX = -L ./minilibx-linux -lmlx -lXext -lX11 -lm

LIBPF = -L ./libft -lft

OBJS = $(SRCS:.c=.o)

all: $(NAME)  

$(NAME): $(OBJS)  
	$(MAKE) -C ./libft
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
