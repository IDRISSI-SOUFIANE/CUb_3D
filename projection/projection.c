/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:40 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/30 15:46:52 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	load_textures(t_data *data)
{
	// Load XPM images into the t_img structs (width/height will be set by mlx)
	data->v_map.east_img.img_ptr = mlx_xpm_file_to_image(data->mlx_ptr, data->v_map.east, &data->v_map.east_img.width, &data->v_map.east_img.height);
	data->v_map.west_img.img_ptr = mlx_xpm_file_to_image(data->mlx_ptr, data->v_map.west, &data->v_map.west_img.width, &data->v_map.west_img.height);
	data->v_map.north_img.img_ptr = mlx_xpm_file_to_image(data->mlx_ptr, data->v_map.north, &data->v_map.north_img.width, &data->v_map.north_img.height);
	data->v_map.south_img.img_ptr = mlx_xpm_file_to_image(data->mlx_ptr, data->v_map.south, &data->v_map.south_img.width, &data->v_map.south_img.height);

	if (!data->v_map.north_img.img_ptr || !data->v_map.south_img.img_ptr ||
		!data->v_map.west_img.img_ptr || !data->v_map.east_img.img_ptr)
	{
		ft_putstr_fd("load_textures: failed to open texture file(s)\n", STDERR_FILENO);
		return (1);
	}

	// Get data addresses for each texture
	data->v_map.east_img.addr = mlx_get_data_addr(data->v_map.east_img.img_ptr, &data->v_map.east_img.bpp, &data->v_map.east_img.line_len, &data->v_map.east_img.endian);
	data->v_map.west_img.addr = mlx_get_data_addr(data->v_map.west_img.img_ptr, &data->v_map.west_img.bpp, &data->v_map.west_img.line_len, &data->v_map.west_img.endian);
	data->v_map.north_img.addr = mlx_get_data_addr(data->v_map.north_img.img_ptr, &data->v_map.north_img.bpp, &data->v_map.north_img.line_len, &data->v_map.north_img.endian);
	data->v_map.south_img.addr = mlx_get_data_addr(data->v_map.south_img.img_ptr, &data->v_map.south_img.bpp, &data->v_map.south_img.line_len, &data->v_map.south_img.endian);

	if (!data->v_map.north_img.addr || !data->v_map.south_img.addr ||
		!data->v_map.west_img.addr || !data->v_map.east_img.addr)
	{
		ft_putstr_fd("load_textures: failed to get texture data address\n", STDERR_FILENO);
		return (1);
	}

	return (0);
}

/*
 * project_wall:
 *   For every ray:
 *     - compute corrected distance (remove fisheye)
 *     - compute wall strip height on screen
 *     - choose the correct texture (N/S/E/W)
 *     - compute texture X coordinate (which column of the texture)
 *     - for each pixel of the vertical strip, compute texture Y and sample color
 *     - draw ceiling, textured wall column, and floor
 */
void project_wall(t_data *data)
{
	int i;

	i = 0;
	while (i < NUM_RAYS)
	{
		// --- 1) correct distance to avoid fisheye
		float rayAngle = data->rays[i].rayAngle;
		float rawDistance = data->rays[i].distance;

		// guard against infinite or zero distance
		if (rawDistance <= 0.0001f || rawDistance == FLT_MAX)
		{
			// draw ceiling + floor for this column and skip texture sample
			int y = 0;
			while (y < HEIGHT / 2)
				my_mlx_pixel_put(data, i, y++, CEILING_COLOR);
			while (y < HEIGHT)
				my_mlx_pixel_put(data, i, y++, FLOOR_COLOR);
			i++;
			continue;
		}

		float correctedDistance = rawDistance * cosf(rayAngle - data->player.rotationangle);

		// --- 2) distance to projection plane
		float distProjPlane = (WIDTH / 2.0f) / tanf(FOV_ANGLE / 2.0f);

		// --- 3) wall strip height
		int wallStripHeight = (int)((TILE_SIZE / correctedDistance) * distProjPlane);
		if (wallStripHeight <= 0)
			wallStripHeight = 1;

		// --- 4) top and bottom of wall on screen
		int wallTopPixel = (HEIGHT / 2) - (wallStripHeight / 2);
		if (wallTopPixel < 0)
			wallTopPixel = 0;
		int wallBottomPixel = (HEIGHT / 2) + (wallStripHeight / 2);
		if (wallBottomPixel >= HEIGHT)
			wallBottomPixel = HEIGHT - 1;

		// --- 5) choose the correct texture based on hit orientation and facing
		t_img *tex = NULL;
		if (data->rays[i].wasHitVertical)
		{
			// Vertical wall => East or West texture
			if (data->rays[i].isRayFacingLeft)
				tex = &data->v_map.west_img; // hit west-facing wall
			else
				tex = &data->v_map.east_img; // hit east-facing wall
		}
		else
		{
			// Horizontal wall => North or South texture
			if (data->rays[i].isRayFacingUp)
				tex = &data->v_map.north_img; // hit north-facing wall
			else
				tex = &data->v_map.south_img; // hit south-facing wall
		}

		// Fallback if texture not loaded
		if (!tex || !tex->addr || tex->width == 0 || tex->height == 0)
		{
			// draw plain colored column (keep previous behavior)
			int y = 0;
			while (y <= wallTopPixel)
				my_mlx_pixel_put(data, i, y++, CEILING_COLOR);
			while (y <= wallBottomPixel)
			{
				int color;
				if (data->rays[i].wasHitVertical)
					color = data->rays[i].isRayFacingLeft ? WEST_COLOR : EAST_COLOR;
				else
					color = data->rays[i].isRayFacingUp ? NORTH_COLOR : SOUTH_COLOR;
				my_mlx_pixel_put(data, i, y++, color);
			}
			while (y < HEIGHT)
				my_mlx_pixel_put(data, i, y++, FLOOR_COLOR);
			i++;
			continue;
		}

		// --- 6) compute texture X coordinate:
		// If vertical hit -> use fractional part of wallHitY, else use fractional part of wallHitX.
		// Map that fractional offset to texture width.
		int texX;
		if (data->rays[i].wasHitVertical)
		{
			float hitY = data->rays[i].wallHitY;
			float offset = fmodf(hitY, TILE_SIZE);               // 0 .. TILE_SIZE-1
			if (offset < 0)
				offset += TILE_SIZE;
			// Map offset to texture width
			texX = (int)((offset / (float)TILE_SIZE) * tex->width);
		}
		else
		{
			float hitX = data->rays[i].wallHitX;
			float offset = fmodf(hitX, TILE_SIZE);               // 0 .. TILE_SIZE-1
			if (offset < 0)
				offset += TILE_SIZE;
			texX = (int)((offset / (float)TILE_SIZE) * tex->width);
		}
		if (texX < 0)
			texX = 0;
		if (texX >= tex->width)
			texX = tex->width - 1;

		// --- 7) draw ceiling
		int y = 0;
		while (y < wallTopPixel)
		{
			my_mlx_pixel_put(data, i, y, data->v_map.n_ceil);
			y++;
		}

		// --- 8) draw textured wall column
		// Calculate how much to step in texture for each screen pixel
		float textureStep = (float)tex->height / (float)wallStripHeight;
		// Starting texture y: if wallTopPixel < 0 we need to start at an offset. But we clamped wallTopPixel >= 0 above,
		// so textureYStart = 0 for wallTopPixel == computed top.
		float textureY = 0.0f;
		// If the wallTopPixel was clamped (when wallStripHeight > HEIGHT) we need to compute the starting textureY:
		if (wallStripHeight > HEIGHT)
		{
			// top of wall would be negative initially; compute how many pixels were skipped at the top
			int off = (wallStripHeight - HEIGHT) / 2;
			textureY = off * textureStep;
		}

		// For each pixel between wallTopPixel and wallBottomPixel, sample texture and draw
		while (y <= wallBottomPixel)
		{
			int texY = (int)textureY;
			if (texY < 0)
				texY = 0;
			if (texY >= tex->height)
				texY = tex->height - 1;

			unsigned int color = get_pixel_color(tex, texX, texY);

			// Optional: simple shading for horizontal vs vertical hits
			if (!data->rays[i].wasHitVertical)
			{
				// slightly darken horizontal hits to give depth (bit-twiddling)
				unsigned int r = (color >> 16) & 0xFF;
				unsigned int g = (color >> 8) & 0xFF;
				unsigned int b = (color) & 0xFF;
				// multiply by 0.8 (approx)
				r = (r * 8) / 10;
				g = (g * 8) / 10;
				b = (b * 8) / 10;
				color = (r << 16) | (g << 8) | b;
			}

			my_mlx_pixel_put(data, i, y, color);
			textureY += textureStep;
			y++;
		}

		// --- 9) draw floor
		while (y < HEIGHT)
		{
			my_mlx_pixel_put(data, i, y, data->v_map.n_floor);
			y++;
		}

		i++;
	}
}
