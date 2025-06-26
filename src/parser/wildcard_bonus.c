/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:17:23 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/24 23:06:06 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	match_pattern(const char *pattern, const char *str)
{
	if (!*pattern && !*str)
		return (1);
	if (*pattern == '*')
	{
		while (*str)
		{
			if (match_pattern(pattern + 1, str))
				return (1);
			str++;
		}
		return (match_pattern(pattern + 1, str));
	}
	if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

static void	add_match_if_valid(char ***matches, const char *pattern,
								struct dirent *entry)
{
	if (entry->d_name[0] == '.')
		return ;
	if (match_pattern(pattern, entry->d_name))
		ft_strarr_add_back(matches, ft_strdup(entry->d_name));
}

static char	**expand_wildcard(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		add_match_if_valid(&matches, pattern, entry);
		entry = readdir(dir);
	}
	closedir(dir);
	if (!matches)
		ft_strarr_add_back(&matches, ft_strdup(pattern));
	else
		ft_strarr_sort(matches);
	return (matches);
}

char	**expand_argument_and_wildcard(const char *arg,
		t_mnsh *shell, int *was_quoted_out)
{
	char	*expanded;
	char	**res;
	char	**wildcards;

	res = NULL;
	expanded = expand_argument(arg, shell, was_quoted_out);
	if (!expanded)
		return (NULL);
	if (!(*was_quoted_out) && ft_strchr(expanded, '*'))
	{
		wildcards = expand_wildcard(expanded);
		if (wildcards)
		{
			ft_strarr_extend(&res, wildcards);
			ft_strarr_free(wildcards);
		}
		free(expanded);
	}
	else
		ft_strarr_add_back(&res, expanded);
	return (res);
}
