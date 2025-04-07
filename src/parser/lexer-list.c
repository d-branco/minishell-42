/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer-list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:22:15 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/07 12:43:47 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Adapted from libft ft_lstiter()
void	tkn_lst_printer(t_token *lst)
{
	const char	*token_string[] = {"WORD", "SINGLE_QUOTE", "DOUBLE_QUOTE",
		"PARENTHESIS_OPEN", "PARENTHESIS_CLOSE", "PIPE", "AND", "OR",
		"OUTPUT_REDIRECTION", "APPEND", "INPUT_REDIRECTION", "HERE_DOC"};

	if (!DEBUG || (lst == NULL))
		return ;
	while (lst)
	{
		ft_printf("--DEBUG--TKN_LIST ======== token at %p\n", lst);
		ft_printf("--DEBUG--TKN_LIST type: %s\n", token_string[lst->type]);
		ft_printf("--DEBUG--TKN_LIST token_string: %s\n", lst->token_string);
		ft_printf("--DEBUG--TKN_LIST next tkn address: %p\n", lst->next);
		lst = lst->next;
	}
	ft_printf("--DEBUG--TKN_LIST ======== END of token list\n");
}

// Adapted from libft
void	tkn_lstclear(t_token **lst)
{
	t_token	*tmp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->token_string)
			free((*lst)->token_string);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

// if malloc fails
//if (tkn == NULL)
//	clean_exit(); when this functions is made we can place malloc above
t_token	*create_token(t_tkn_type token_type, char *str)
{
	t_token	*tkn;

	tkn = (t_token *)ft_malloc(sizeof(t_token) * 1);
	tkn->type = token_type;
	tkn->token_string = str;
	tkn->next = NULL;
	return (tkn);
}

// Adapted from libft
void	tkn_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}
