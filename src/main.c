/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/07/17 09:46:32 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	guide(void)
{
	printf("Bem vindo ao nosso mini Ray Tracing!\n\n");
	printf("Este é um guia para poderes interagir com a cena\n\n");
	printf("Câmera\n");
	printf("Use as teclas 'A, W, D, S, Q e E' fazer movimentos de translação ");
	printf("com a câmera\n");
	printf("Use as teclas 'J, I, L, e K' fazer movimentos de rotação ");
	printf("com a câmera\n\n");
	printf("Objetos\n");
	printf("Use 'Tab' para alternar sobre os objetos na cena!\n");
	printf("Use as teclas 'rigth, up, left, down, U e N' fazer movimentos ");
	printf("de translação com os objetos\n");
	printf("Use as teclas 'X, Y e Z' fazer movimentos de rotação com os ");
	printf("objetos(Cilindros e planos)\n\n");
	printf("Bom proveito!\n\n");
}

int	is_a_rt_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 3)
		return (0);
	if (filename[len - 3] == '.' && filename[len - 2] == 'r' && filename[len
			- 1] == 't')
		return (1);
	fprintf(stderr, "O ficheiro %s não tem a extensão .rt\n", filename);
	fprintf(stderr, "Por favor, use um ficheiro com a extensão .rt\n");
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	create_data(&data);
	if (ac == 2)
	{
		if (!is_a_rt_extension(av[1]) || parse_rt_file(av[1], &data) != 0
			|| data.invalid_line)
			return (1);
	}
	else
		return (1 * fprintf(stderr, "Arquivo '.rt' não inserido\n"));
	data.render = (t_render *)malloc(sizeof(t_render));
	if (!data.render)
		return (1 * fprintf(stderr, "Erro ao alocar memória para render\n"));
	collect_mem(data.render);
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "miniRT");
	init_image(&data.img, data.mlx, WIDTH, HEIGHT);
	data.camera->dir = look_at(data.camera->origin, (t_vector){0, 1, 0});
	render_scene(&data);
	guide();
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 17, 1L << 17, close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}
