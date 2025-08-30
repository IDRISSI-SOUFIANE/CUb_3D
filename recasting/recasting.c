/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recasting.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:44:23 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/30 16:13:27 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

float distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	float dx;
	float dy;

	dx = x2 - x1;
	dy = y2 - y1;
	return (sqrt((dx * dx) + (dy * dy)));
}

int calculate_width(char **map)
{
	int max_w;
	int cur_w;
	int i;

	max_w = 0;
	i = 0;
	while (map[i])
	{
		cur_w = 0;
		while (map[i][cur_w])
			cur_w++;
		if (cur_w > max_w)
			max_w = cur_w;
		i++;
	}
	return (max_w);
}

int calculate_height(char **map)
{
	int h;

	h = 0;
	while (map[h])
		h++;
	return (h);
}

float normalizeAngle(float angle)
{
	float res;

	res = fmod(angle, 2 * M_PI);
	if (res < 0)
		res += (2 * M_PI);
	return (res);
}

int has_wall_at(t_data *data, float x, float y)
{
	int gx;
	int gy;
	int h;

	gx = floor(x / TILE_SIZE);
	gy = floor(y / TILE_SIZE);
	h = calculate_height(data->map);
	if (gy < 0 || gy >= h)
		return (1);
	if (gx < 0 || gx >= (int)ft_strlen(data->map[gy]))
		return (1);
	return (data->map[gy][gx] == '1');
}

void	set_ray_direction(t_ray *ray, float angle)
{
	ray->isRayFacingDown = angle > 0 && angle < M_PI;
	ray->isRayFacingUp = !ray->isRayFacingDown;
	ray->isRayFacingRight = angle < 0.5 * M_PI || angle > 1.5 * M_PI;
	ray->isRayFacingLeft = !ray->isRayFacingRight;
}

static void init_hit(t_hit *hit)
{
	hit->dist = FLT_MAX;
	hit->x = 0;
	hit->y = 0;
}

t_ray_params	set_horz_params(t_data *data, t_ray *ray, float angle)
{
	t_ray_params	p;

	p.yintercept = floor(data->player.y / TILE_SIZE) * TILE_SIZE;
	if (ray->isRayFacingDown)
		p.yintercept += TILE_SIZE;
	p.xintercept = data->player.x + (p.yintercept - data->player.y) / tan(angle);
	p.ystep = TILE_SIZE;
	if (ray->isRayFacingUp)
		p.ystep *= -1;
	p.xstep = TILE_SIZE / tan(angle);
	if (ray->isRayFacingLeft && p.xstep > 0)
		p.xstep *= -1;
	if (ray->isRayFacingRight && p.xstep < 0)
		p.xstep *= -1;
	return (p);
}

t_ray_params	set_ver_params(t_data *data, t_ray *ray, float angle)
{
	t_ray_params	p;

	p.xintercept = floor(data->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray->isRayFacingRight)
		p.xintercept += TILE_SIZE;
	p.yintercept = data->player.y + (p.xintercept - data->player.x) * tan(angle);
	p.xstep = TILE_SIZE;
	if (ray->isRayFacingLeft)
		p.xstep *= -1;
	p.ystep = TILE_SIZE * tan(angle);
	if (ray->isRayFacingUp && p.ystep > 0)
		p.ystep *= -1;
	if (ray->isRayFacingDown && p.ystep < 0)
		p.ystep *= -1;
	return (p);
}

static void get_horz_hit(t_data *data, t_ray *ray,
						 t_ray_params p, t_hit *hit)
{
	float nx;
	float ny;

	nx = p.xintercept;
	ny = p.yintercept;
	if (ray->isRayFacingUp)
		ny -= 0.002;
	while (nx >= 0 && nx <= calculate_width(data->map) * TILE_SIZE
			&& ny >= 0 && ny <= calculate_height(data->map) * TILE_SIZE)
	{
		if (has_wall_at(data, nx, ny))
		{
			hit->x = nx;
			hit->y = ny;
			hit->dist = distanceBetweenPoints(data->player.x,
											  data->player.y, nx, ny);
			return;
		}
		nx += p.xstep;
		ny += p.ystep;
	}
}

static void get_ver_hit(t_data *data, t_ray *ray,
						t_ray_params p, t_hit *hit)
{
	float nx;
	float ny;

	nx = p.xintercept;
	ny = p.yintercept;
	if (ray->isRayFacingLeft)
		nx -= 0.002;
	while (nx >= 0 && nx <= calculate_width(data->map) * TILE_SIZE && ny >= 0 && ny <= calculate_height(data->map) * TILE_SIZE)
	{
		if (has_wall_at(data, nx, ny))
		{
			hit->x = nx;
			hit->y = ny;
			hit->dist = distanceBetweenPoints(data->player.x,
											  data->player.y, nx, ny);
			return;
		}
		nx += p.xstep;
		ny += p.ystep;
	}
}

static void choose_hit(t_ray *ray, t_hit horz, t_hit ver)
{
	if (horz.dist < ver.dist)
	{
		ray->distance = horz.dist;
		ray->wasHitVertical = 0;
		ray->wallHitX = horz.x;
		ray->wallHitY = horz.y;
	}
	else
	{
		ray->distance = ver.dist;
		ray->wasHitVertical = 1;
		ray->wallHitX = ver.x;
		ray->wallHitY = ver.y;
	}
}

/*
** Main Raycasting
*/

int	castRay(t_data *data, int col, float angle)
{
	t_hit			horz;
	t_hit			ver;
	t_ray_params	h;
	t_ray_params	v;

	set_ray_direction(&data->rays[col], angle);
	init_hit(&horz);
	init_hit(&ver);
	h = set_horz_params(data, &data->rays[col], angle);
	v = set_ver_params(data, &data->rays[col], angle);
	get_horz_hit(data, &data->rays[col], h, &horz);
	get_ver_hit(data, &data->rays[col], v, &ver);
	data->rays[col].rayAngle = angle;
	choose_hit(&data->rays[col], horz, ver);
	if (data->rays[col].distance == FLT_MAX)
		return (0);
	return (1);
}

void	castAllRays(t_data *data)
{
	int		i;
	float	angle;

	i = 0;
	angle = data->player.rotationangle - (FOV_ANGLE / 2);
	while (i < NUM_RAYS)
	{
		angle = normalizeAngle(angle);
		castRay(data, i, angle);
		angle += FOV_ANGLE / NUM_RAYS;
		i++;
	}
}
