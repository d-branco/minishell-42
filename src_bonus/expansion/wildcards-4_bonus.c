/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards-4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:29:14 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:29:24 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

void	find_wildcard_matches(char **ret, char *str)
{
	struct dirent	*dir_entry;
	DIR				*dir;
	t_list			*words;

	if (check_iwatod(str, ret) == -1)
		return ;
	dir = opendir(".");
	if (dir == NULL)
	{
		*ret = ft_strjoin(str, " ");
		return ;
	}
	words = NULL;
	dir_entry = readdir(dir);
	while (dir_entry != NULL)
	{
		if (match_wildcard(dir_entry->d_name, str) == 1)
			ft_lstadd_back(&words,
				ft_lstnew((char *)ft_strjoin(dir_entry->d_name, " ")));
		dir_entry = readdir(dir);
	}
	check_empty_and_sort(&words, ret, str);
	ft_lstclear(&words, free);
	closedir(dir);
}

int	check_iwatod(char *str, char **ret)
{
	if (str == NULL || *str == '\0')
		return (-1);
	if (!contains_wildcards(str))
	{
		*ret = ft_strjoin(str, " ");
		return (-1);
	}
	return (0);
}

int	contains_wildcards(char *str)
{
	int				i;
	t_quote_state	state;

	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (str[i] && (str[i] != '*'
			|| state.escaped || state.single_quote || state.double_quote))
	{
		handle_quote((str + i), &state);
		i++;
	}
	return (str[i] == '*');
}
