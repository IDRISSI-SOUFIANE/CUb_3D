/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:35:12 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/26 16:08:56 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../include/cub3d.h"

static int	to_trim(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}


char	*ft_strtrim(char *s1, char *set)
{
	int		i;
	int		j;

	if (!s1 || !set)
		return (NULL);
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));

	i = 0;
	while (s1[i] && to_trim(s1[i], set))
		i++;

	j = ft_strlen(s1);
	while (j > i && to_trim(s1[j - 1], set))
		j--;

	return (ft_substr(s1, i, j - i));
}