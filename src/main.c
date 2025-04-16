/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:56:40 by fjilaias          #+#    #+#             */
/*   Updated: 2025/02/11 10:22:02 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int main(int ac, char *av[])
{
    (void) ac;
    (void) av;
    t_scene *scene;
    scene = create_scene();


    printf("Hello MiniRT %li\n", ft_strlen("1973"));
    printf("A potencia de 5 evlevado a 3 é %f\n", pow(5, 3));

    int rs = parse_rt_file("models/test.rt", scene);
    printf("RESULT: %d", rs);

    void    *mlx;
    void    *win;
    t_image img;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 800, 600, "miniRT");

    init_image(&img, mlx, 800, 600);

    // pinta um quadrado vermelho no canto superior esquerdo
    for (int y = 0; y < 100; y++)
        for (int x = 0; x < 100; x++)
            put_pixel(&img, x, y, 0x00FF0000); // vermelho

    mlx_put_image_to_window(mlx, win, img.img_ptr, 0, 0);
    mlx_loop(mlx);
    return (0);
}
