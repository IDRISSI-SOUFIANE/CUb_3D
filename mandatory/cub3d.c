/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:08:33 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/30 15:25:45 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

char *_join_lines()
{
	int fd;
	char *line;
	char *temp;
	char *res;

	res = NULL;
	fd = open("map/map.cub", O_RDONLY, 0777);
	if (fd < 0)
		return (ft_putstr_fd("fd error\n", STDERR_FILENO), NULL);
	temp = NULL;
	while ((line = get_next_line(fd)) != NULL)
	{
		res = ft_strjoin(temp, line);
		if (!res)
			return (ft_putstr_fd("res fail\n", STDERR_FILENO),
					free(temp), free(line), NULL);
		free(line);
		free(temp);
		temp = res;
	}
	return (close(fd), res);
}
int ft_start(t_data *data)
{
	char *join_lines;

	join_lines = _join_lines();
	if (!join_lines)
		return (1);
	data->map = ft_split(join_lines, "\n");
	if (!data->map)
		return (free(join_lines), 1);
	if (init_window(data))
		return (1337);
	return (free_2d_array(data->map), free(join_lines), 0);
}
