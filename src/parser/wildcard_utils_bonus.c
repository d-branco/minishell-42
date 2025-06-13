/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:42:01 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/12 11:42:06 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_strarr_add_back(char ***arr, char *new_str)
{
	int		len;
	int		i;
	char	**new_arr;

	len = 0;
	i = -1;
	if (!arr)
		return ;
	while (*arr && (*arr)[len])
		len++;
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return ;
	while (++i < len)
		new_arr[i] = (*arr)[i];
	new_arr[len] = new_str;
	new_arr[len + 1] = NULL;
	free(*arr);
	*arr = new_arr;
}

void	ft_strarr_extend(char ***dest, char **src)
{
	int		i;

	i = 0;
	while (src && src[i])
	{
		ft_strarr_add_back(dest, ft_strdup(src[i]));
		i++;
	}
}

void	ft_strarr_free(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	ft_strarr_sort(char **arr)
{
	char	*temp;
	int		i;
	int		j;

	i = -1;
	if (!arr)
		return ;
	while (arr[++i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
			j++;
		}
	}
}
