/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:43:58 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/30 15:43:59 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* khasni n  fix headerfile */

#include "../include/cub3d.h"

// Helper function to get a pixel color from a texture image
// This is crucial for sampling individual pixels from loaded textures
unsigned int get_pixel_color(t_img *img, int x, int y)
{
	char *pixel_addr;

	// Check bounds to prevent reading outside the image data
	// printf("img->width: %d | img->height: %d\n", img->width, img->height);
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return (0); // Return black or transparent for out-of-bounds access

	// Calculate the memory address of the pixel
	pixel_addr = img->addr + (y * img->line_len + x * (img->bpp / 8));
	return (*(unsigned int *)pixel_addr);
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char *dst;

	// Check if coordinates are within the window bounds
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;
	// Calculate the memory address of the pixel
	dst = data->img.addr + (y * data->img.line_len + x * (data->img.bpp / 8));
	// Write the color to the pixel address
	*(unsigned int *)dst = color;
}

// void draw_wall(t_data *data, int tile_x, int tile_y)
// {
// 	int x;
// 	int y;
// 	// unsigned int color;

// 	y = 0;
// 	while (y < TILE_SIZE)
// 	{
// 		x = 0;
// 		while (x < TILE_SIZE)
// 		{
// 			// Since you don't have texture loading working yet,
// 			// just use a solid color for walls
// 			my_mlx_pixel_put(data, tile_x + x, tile_y + y, GREEN);
// 			x++;
// 		}
// 		y++;
// 	}
// }

// // Draws a background tile (floor) at the given tile coordinates.
// void draw_background(t_data *data, int tile_x, int tile_y)
// {
// 	int x;
// 	int y;

// 	x = 0;
// 	while (x < TILE_SIZE)
// 	{
// 		y = 0;
// 		while (y < TILE_SIZE)
// 		{
// 			my_mlx_pixel_put(data, ((tile_x + x)), ((tile_y + y)), WHITE); // Floor color
// 			y++;
// 		}
// 		x++;
// 	}
// }

// void draw(t_data *data, int i, int j)
// {
// 	int tile_x;
// 	int tile_y;

// 	tile_x = j * TILE_SIZE;
// 	tile_y = i * TILE_SIZE;

// 	draw_background(data, tile_x, tile_y);

// 	if (data->data->map[i][j] == '1')
// 	{
// 		draw_wall(data, tile_x, tile_y);
// 	}
// 	// Player 'N' character is used for initial position, but not for drawing here.
// }

// // Renders the entire 2D data->map view.
// void render_data->map(t_data *data)
// {
// 	int i;
// 	int j;

// 	i = 0;
// 	while (data->data->map[i])
// 	{
// 		j = 0;
// 		while (data->data->map[i][j])
// 		{
// 			draw(data, i, j); // Draw each tile
// 			j++;
// 		}
// 		i++;
// 	}
// 	// The player is drawn separately in game_loop after rendering the data->map.
// }

// void draw_line(t_data *data, int x0, int y0, float angle, int length)
// {
// 	int i;
// 	int x;
// 	int y;

// 	i = 0;
// 	while (i < length)
// 	{
// 		// Calculate the next point on the line using trigonometry
// 		x = x0 + cos(angle) * i;
// 		y = y0 + sin(angle) * i;
// 		// Draw the pixel if it's within bounds
// 		if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
// 			my_mlx_pixel_put(data, x, y, GREEN); // Player direction line color
// 		i++;
// 	}
// }

// // // Draws the player's representation (the red line) on the data->map.
// void draw_player(t_data *data)
// {
// 	// Convert player's world coordinates to screen coordinates for drawing
// 	data->player.screen_x = (int)(data->player.x);
// 	data->player.screen_y = (int)(data->player.y);

// 	// Draw player direction line
// 	draw_line(data,
// 			  data->player.screen_x,
// 			  data->player.screen_y,
// 			  data->player.rotationangle,
// 			  TILE_SIZE); // TILE_SIZE / 2
// }

int key_press(int keycode, t_data *data)
{
	if (keycode == 13 || keycode == 126)
		data->player.walkdirection = 1; // up
	else if (keycode == 1 || keycode == 125)
		data->player.walkdirection = -1; // down
	else if (keycode == 124)
		data->player.turndirection = 1; // ->
	else if (keycode == 123)
		data->player.turndirection = -1; // <-
	else if (keycode == 53)				 // ESC
		(mlx_destroy_window(data->mlx_ptr, data->win_ptr), exit(0));
	return (0);
}

// Handles key release events. Resets player's walk and turn directions.
int key_release(int keycode, t_data *data)
{
	if ((keycode == 13 || keycode == 126) && data->player.walkdirection == 1)
		data->player.walkdirection = 0;
	else if ((keycode == 1 || keycode == 125) && data->player.walkdirection == -1)
		data->player.walkdirection = 0;
	else if (keycode == 124 && data->player.turndirection == 1)
		data->player.turndirection = 0;
	else if (keycode == 123 && data->player.turndirection == -1)
		data->player.turndirection = 0;
	return (0);
}

// int close_window(t_data *data)
// {
// 	// should close fd
// 	// free the leaks
// 	mlx_destroy_image(data->mlx_ptr, data->img.img_ptr);
// 	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
// 	exit(0);
// }


int close_window(t_data *data)
{
    if (data->wall.img_ptr)
        mlx_destroy_image(data->mlx_ptr, data->wall.img_ptr);
    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx_ptr, data->img.img_ptr);
    if (data->win_ptr)
        mlx_destroy_window(data->mlx_ptr, data->win_ptr);

    free_all(data);
    exit(0);
}


void handle_event(t_data *data)
{
	mlx_hook(data->win_ptr, 2, 1L << 0, key_press, data);
	mlx_hook(data->win_ptr, 3, 1L << 1, key_release, data);
	mlx_hook(data->win_ptr, 17, 0, close_window, data);
}

void cleanup(t_data *data)
{
	if (data->wall.img_ptr)
		mlx_destroy_image(data->mlx_ptr, data->wall.img_ptr);
	if (data->img.img_ptr)
		mlx_destroy_image(data->mlx_ptr, data->img.img_ptr);
	if (data->win_ptr)
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
}

// Check if a point is a wall
int has_wall_at_1337(t_data *data, float x, float y)
{
	int mapGridIndexX;
	int mapGridIndexY;

	// Check if the coordinates are within the data->map bounds
	if (x < 0 || x > WIDTH * TILE_SIZE || y < 0 || y > HEIGHT * TILE_SIZE)
		return 1; // Out of bounds is a wall

	// Convert pixel coordinates to data->map grid coordinates
	mapGridIndexX = floor(x / TILE_SIZE);
	mapGridIndexY = floor(y / TILE_SIZE);

	// Check for wall
	if (data->map[mapGridIndexY][mapGridIndexX] == '1')
		return 1;
	return 0;
}

// Moves the player based on input and handles basic collision detection.
void move_player(t_data *data)
{
	float	move_step;
	float	new_x;
	float	new_y;
	float	player_radius;

	// Update player rotation based on turn direction and speed
	data->player.rotationangle += data->player.turndirection * data->player.turnspeed;
	data->player.rotationangle = normalizeAngle(data->player.rotationangle);


	// printf("Rotation angle: %.2f rad (%.1f°)\n",
	//        data->player.rotationangle,
	//        data->player.rotationangle * (180.0 / M_PI));

	// Calculate potential movement vector
	move_step = data->player.walkdirection * data->player.walkspeed;
	new_x = data->player.x + cos(data->player.rotationangle) * move_step;
	new_y = data->player.y + sin(data->player.rotationangle) * move_step;

	// Use a small radius for collision detection to represent the player's bounding box
	player_radius = TILE_SIZE / 12;

	// Check for a wall at the new potential position. We check four points
	// around the player's circumference to prevent them from passing through corners.
	if (!has_wall_at_1337(data, new_x + player_radius, new_y) &&
		!has_wall_at_1337(data, new_x - player_radius, new_y) &&
		!has_wall_at_1337(data, new_x, new_y + player_radius) &&
		!has_wall_at_1337(data, new_x, new_y - player_radius))
	{
		data->player.x = new_x;
		data->player.y = new_y;
	}

	/*
		new_x + player_radius, new_y: The point to the right of the new position.

		new_x - player_radius, new_y: The point to the left of the new position.

		new_x, new_y + player_radius: The point below the new position.

		new_x, new_y - player_radius: The point above the new position.

		The player_radius value acts as a small buffer, representing the player's physical size.
		By checking these four points, the code effectively creates a small box around the player's body.
		If any one of these four points is about to move into a wall tile, the entire movement is canceled.
		This ensures the player is correctly blocked before their bounding box can intersect with the wall's bounding box.
		This method is far more robust and accurately simulates a solid object moving within the data->map, preventing the clipping issues you were experiencing.
	*/
}

// Initializes the t_data and t_player structures, and loads textures.
void init_struct(t_data *data)
{
	int i;
	int j;

	data->player.x = 0;
	data->player.y = 0;
	// data->player.screen_x = 0;
	// data->player.screen_y = 0;
	// data->player.radius = 3; // why need this `radius`?
	data->player.turndirection = 0;
	data->player.walkdirection = 0;
	data->player.rotationangle = 0; // Will be set based on player char
	data->player.walkspeed = 3.5f;
	data->player.turnspeed = 5 * (M_PI / 180) /*0.3f*/;

	// Find player's initial position and set rotation angle
	i = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' || data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				// Player position centered within the tile
				data->player.x = (j + 0.5f) * TILE_SIZE;
				data->player.y = (i + 0.5f) * TILE_SIZE;
				/*foor check --->*/ printf("data->player.x: %f | data->player.y : %f\n", data->player.x, data->player.y);
				if (data->map[i][j] == 'N')
					data->player.rotationangle = 3 * M_PI / 2; // Facing North (up)
				else if (data->map[i][j] == 'S')
					data->player.rotationangle = M_PI / 2; // Facing South (down)
				else if (data->map[i][j] == 'E')
					data->player.rotationangle = 0; // Facing East (right)
				else if (data->map[i][j] == 'W')
					data->player.rotationangle = M_PI; // Facing West (left)
				break;								   // Found player, no need to continue data->map scan for player pos
			}
			j++;
		}
		i++;
	}
}

int game_loop(t_data *data)
{
	// ft_memset(data->img.addr, 0, WIDTH * HEIGHT * (data->img.bpp / 8));
	move_player(data);
	castAllRays(data);
	project_wall(data);

	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.img_ptr, 0, 0);
	return (0);
}

int init_window(t_data *data)
{
	data->mlx_ptr = mlx_init();
	if ((WIDTH > 8192 || HEIGHT > 8192) || (WIDTH <= 0 || HEIGHT <= 0))
		return (ft_putstr_fd("window not correct\n", STDERR_FILENO), 1337); // check leaks!!
	if (!data->mlx_ptr)
		exit(1);
	init_struct(data);
	if (load_textures(data))
		(cleanup(data), exit(1));
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIDTH, HEIGHT, "CUB3D");
	if (!data->win_ptr)
		return (cleanup(data), 1337);
	data->img.img_ptr = mlx_new_image(data->mlx_ptr, WIDTH, HEIGHT);
	if (!data->img.img_ptr)
		return (cleanup(data), 1337);
	data->img.addr = mlx_get_data_addr(data->img.img_ptr, &data->img.bpp,
									   &data->img.line_len, &data->img.endian);
	if (!data->img.addr)
		return (cleanup(data), 1337);
	handle_event(data);
	mlx_loop_hook(data->mlx_ptr, game_loop, data);
	mlx_loop(data->mlx_ptr);

	return (0);
}
