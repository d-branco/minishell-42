/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-14.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:16:42 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:16:57 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	make_tkn_lst(t_token **lst, char *str)
{
	t_token	*end;
	t_token	*tmp;

	*lst = NULL;
	while (1)
	{
		if (lexer(&tmp, &str) != 0)
		{
			tkn_free_one(tmp);
			free_lst_tkn(*lst);
			return (-1);
		}
		if (*lst == NULL)
		{
			*lst = tmp;
			end = *lst;
		}
		else
		{
			end->next = tmp;
			end = end->next;
		}
		if (tmp->type == e_END)
			return (0);
	}
}

void	free_lst_tkn(t_token *lst)
{
	t_token	*previous;

	previous = 0;
	while (lst)
	{
		tkn_free_one(previous);
		previous = lst;
		lst = lst->next;
	}
	tkn_free_one(previous);
}

void	tkn_free_one(t_token *tkn)
{
	if (tkn)
		free(tkn->str);
	free(tkn);
}

void	next_token(t_token **list)
{
	*list = (*list)->next;
}
