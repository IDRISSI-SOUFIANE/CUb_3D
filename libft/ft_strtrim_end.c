/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_end.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:14:42 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/25 15:14:43 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

char	*ft_strtrim_end(char *s1, char *set)
{
	int end;

	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1) - 1;
	while (end >= 0 && s1[end] == ' ')
	{
		end--;
	}
	return (ft_substr(s1, 0, end + 1));
}