/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards-3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:28:07 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:28:20 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

void	check_empty_and_sort(t_list **words, char **ret, char *str)
{
	char	*escaped;

	if (*words == NULL)
		*ret = ft_strjoin(str, " ");
	else
	{
		lst_bubble_sort(words, ft_strcmp);
		*ret = lst_to_str(*words);
		escaped = backslash_chars(*ret, 0);
		free(*ret);
		*ret = escaped;
	}
}

char	*backslash_chars(char *str, int flag)
{
	int		i;
	char	*res;
	char	*chars;

	if (flag == 1)
		chars = "\"\\$";
	else
		chars = "\"\\$'";
	res = ft_malloc(sizeof(char) * (2 * ft_strlen(str) + 1));
	i = 0;
	while (*str)
	{
		if (ft_strchr(chars, *str))
		{
			res[i] = '\\';
			i++;
		}
		res[i] = *str;
		i++;
		str++;
	}
	res[i] = 0;
	return (res);
}

void	lst_bubble_sort(t_list **lst, int (cmp)(const char *, const char *))
{
	int		sorted;
	t_list	*current;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		current = *lst;
		while (current && current->next)
		{
			if (0 < cmp(current->content, current->next->content))
			{
				sorted = 0;
				lst_swap(current, current->next);
			}
			current = current->next;
		}
	}
}

t_list	*lst_swap(t_list *a, t_list *b)
{
	void	*tmp;

	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
	return (b);
}
