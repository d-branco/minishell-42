/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/06 10:18:02 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	syntax error
	Deal with normal command
			normal commands + arguments
	Redirections <, <<, > and >>
	Pipelines |
	Logical operators && and ||
	Quotes (" and ')
*/

void	tkn_lst_printer(t_token *lst);

int	parser(char *input)
{
	t_token	*list_o_tokens;

	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	tkn_lst_printer(list_o_tokens);
	tkn_lstclear(&list_o_tokens);
	return (0);// temp
}

// Adapted from libft ft_lstiter()
void	tkn_lst_printer(t_token *lst)
{
	const char	*token_string[] = {"string"};

	if (!DEBUG || (lst == NULL))
		return ;
	while (lst)
	{
		ft_printf("--DEBUG--TKN_LIST ======== token at %p\n", lst);
		ft_printf("--DEBUG--TKN_LIST type: %s\n", token_string[lst->type]);
		ft_printf("--DEBUG--TKN_LIST token_string: %s\n", lst->token_string);
		ft_printf("--DEBUG--TKN_LIST Next token: %p\n", lst->next);
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

// Creates a tokens and returns 0 if success
int	parse_input_into_token_list(t_token **list, char *input)
{
	int	pos;

	pos = 0;
	while (input[pos])
	{
		while (ft_isspace(input[pos]))
			pos++;
		if (!input[pos])
			break ;
		get_token(list, input, &pos);
	//	if (DEBUG)
	//		ft_printf("--DEBUG-- TOKEN	type:	%i\n"
	//			"--DEBUG-- TOKEN	str:	%s\n",
	//			(*list)->type, (*list)->token_string);
	//	free((*list)->token_string);
	//	free(*list);
	}
	return (0); //temp
}

void	get_token(t_token **list, char *input, int *pos)
{
	char	*tkn_str;

	//check the type of token
	//if string
	isolate_word_token(input, pos, &tkn_str);
	//else
	//isolate_operator_token();
	//then
	//add token to list

	//*list = create_token(e_word, tkn_str);
	tkn_lstadd_back(list, create_token(e_word, tkn_str));
	if (DEBUG)
		ft_printf("--DEBUG-- Got token: %s\n", tkn_str);
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

//	char	*ft_substr(char const *s, unsigned int start, size_t len);
void	isolate_word_token(char *input, int *pos, char **token_string)
{
	int	i;

	i = 0;
	while (input[*pos + i] && !ft_isspace(input[*pos + i])
		&& input[*pos + i] != '\'' && input[*pos + i] != '\"')
	{
		i++;
	}
	*token_string = ft_substr(input, *pos, i);
	*pos += i;
}

////typedef struct s_token
////{
////	t_token_type	type;
////	char			*token_string;
////}	t_token;
t_token	*create_token(t_token_type token_type, char *str)
{
	t_token	*tkn;

	tkn = (t_token *)ft_malloc(sizeof(t_token) * 1);
	tkn->type = token_type;
	tkn->token_string = str;
	tkn->next = NULL;
	return (tkn);
}
