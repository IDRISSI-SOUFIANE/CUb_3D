/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:43:37 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/30 16:13:52 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include "mlx.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MLX_ERROR 1

#define NORTH_COLOR 0x00FF0000 // Red
#define SOUTH_COLOR 0x0000FF00 // Green
#define WEST_COLOR  0x000000FF // Blue
#define EAST_COLOR  0x00FFFF00 // Yellow
#define CEILING_COLOR 0x0087CEEB // Light Sky Blue
#define FLOOR_COLOR 0x00808080 // Grey

#define RED 0x00FF0000
#define GREEN 0x0000FF00
#define WHITE 0xFF0000
#define BLUE 0x000000FF
#define BLACK 0x00000000


#define TILE_SIZE 32

// FIXED: Using fixed window size as requested
#define WIDTH 1000
#define HEIGHT 1200
#define FOV_ANGLE 60 * (M_PI / 180)
#define NUM_RAYS (WIDTH) // The number of rays should correspond to the screen width

#define WALL_STRIP_WIDTH 1
// #define NUM_RAYS WIDTH / WALL_STRIP_WIDTH // The number of rays should correspond to the screen width


#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>



/*============= Execution ===================*/

typedef struct s_ray
{
	float   rayAngle;
	float   wallHitX;
	float   wallHitY;
	float   distance;
	int     wasHitVertical;
	int     isRayFacingDown;
	int     isRayFacingUp;
	int     isRayFacingRight;
	int     isRayFacingLeft;
} t_ray;

typedef struct s_img
{
	void *img_ptr;
	char *addr;
	int bpp;
	int line_len;
	int endian;
	int width;
	int height;
} t_img;

typedef struct s_player
{
	float x;
	float y;
	// int screen_x;
	// int screen_y;
	float radius;
	float turndirection;
	float walkdirection;
	float rotationangle;
	float walkspeed;
	float turnspeed;
} t_player;


/*============= Parsing ===================*/

typedef struct s_vmap
{
	char *path;
	char *str;
	char *color;
	char *north;
	char *south;
	char *west;
	char *east;
	char *floor;
	char *ceil;
	int n_floor;
	int n_ceil;

	// second_map
	char	*_join_map_lines;// should free()

	 // **NEW:** Add a struct for each texture image
	t_img	north_img;
	t_img	south_img;
	t_img	west_img;
	t_img	east_img;
}	t_vmap;


typedef struct s_data
{
	void *win_ptr;
	void *mlx_ptr;
	char **map;
	int map_width;
	int map_height;
	t_img img;
	t_img wall;
	t_vmap v_map;
	t_player player;
	t_ray rays[NUM_RAYS];
} t_data;

/* ============Reacsting======== */
typedef struct s_ray_params
{
	float yintercept;
	float xintercept;
	float ystep;
	float xstep;
} t_ray_params;

typedef struct s_hit
{
	float dist;
	float x;
	float y;
} t_hit;
/* ============Reacsting======== */

/*============= Parsing ===================*/

/*============= Execution ===================*/

/*=============get_next_line (Start)==========================*/
char	*get_next_line(int fd);

char	*read_file(int fd, char *left_over, char *buffer);
char	*extr_line(char *line);
char	*ft_left_over(char *line);
/*=============get_next_line (End)============================*/

/*=============libft (Start)============================*/
void	ft_putstr_fd(char *s, int fd);
int		ft_strlen(const char *s);
char	*ft_strdup(char *s1);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strchr(char *s, char c);
char	**ft_split(char const *s, char *sep);
void	free_2d_array(char **arr);
int		ft_count_2d_array(char **arr);
void	*ft_memset(void *b, int c, size_t len);
int		ft_strcmp(char *s1, char *s2);
long	ft_atoi(char *str);
int		ft_isdigit(int c);
char 	*ft_strtrim(char *s1, char *set);
char	*ft_strtrim_end(char *s1, char *set);
char	*ft_strstr(char *str, char *to_find);
/*=============libft (End)============================*/

/*=============Parsing (End)============================*/
int ft_parsing(char *map, t_data *data);
int		check_extention(char *map);
int check_first_part(int fd, int count, t_data *data);
/*=============Parsing (Start)============================*/

/*=============start_drawing (Start)============================*/
int		init_window(t_data *data);
void	draw_ray(t_data *data, int x0, int y0, int x1, int y1);
/*=============start_drawing (End)============================*/

/*============= Recasting (Start)============================*/
float	normalizeAngle(float angle);
int		castRay(t_data *data, int columId, float rayAngle);
void	castAllRays(t_data *data);
float	distanceBetweenPoints(float x1, float y1, float x2, float y2);
int		has_wall_at(t_data *data, float x, float y);
/*============= Recasting (End)============================*/


/*============= Recasting (Start)============================*/
void	project_wall(t_data *data);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
/*============= Recasting (End)============================*/


int		load_textures(t_data *data);
unsigned int get_pixel_color(t_img *img, int x, int y);
int has_wall_at_1337(t_data *data, float x, float y);
int ft_start(t_data *data);


void free_2d_array(char **arr);
void free_all(t_data *data);

#endif
