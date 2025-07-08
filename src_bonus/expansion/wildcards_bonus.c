/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:57:49 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 16:59:46 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	check_starting_wildcard(int *i, char **file, char *expr, char ***sections)
{
	char	*tmp;

	tmp = quote_remove(expr);
	if (*file[0] == '.' && tmp[0] != '.')
	{
		free_strarr(*sections);
		free(tmp);
		return (1);
	}
	free(tmp);
	if (expr[0] != '*')
	{
		if (0 != ft_strncmp(*file, *sections[0], ft_strlen(*sections[0])))
		{
			free_strarr(*sections);
			return (1);
		}
		*file = *file + ft_strlen(*sections[0]);
		*i = *i + 1;
	}
	return (0);
}

int	check_ending_wildcard(int i, char *file, char *expr, char **sections)
{
	if (expr[ft_strlen(expr) - 1] != '*')
	{
		file -= ft_strlen(sections[i - 1]);
		file += ft_strlen(file) - ft_strlen(sections[i - 1]);
		if (!*sections[i - 1] || ft_strcmp(file, sections[i - 1]) != 0)
			return (0);
	}
	return (TRUE);
}

void	quote_remove_strarr(char **strarr)
{
	int		i;
	char	*tmp;

	i = 0;
	while (strarr[i])
	{
		tmp = quote_remove(strarr[i]);
		free(strarr[i]);
		strarr[i] = tmp;
		i++;
	}
}

int	check_single_section(int i, int *ret, char **file, char **sections)
{
	char	*tmp;

	if (sections[i] == NULL)
	{
		*ret = 1;
		return (0);
	}
	tmp = ft_strnstr(*file, sections[i], ft_strlen(*file));
	if (tmp == NULL)
	{
		*ret = 0;
		return (0);
	}
	*file = tmp + ft_strlen(sections[i]);
	return (1);
}
