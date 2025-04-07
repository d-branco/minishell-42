/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/07 10:14:09 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser(char *input)
{
	t_token	*list_o_tokens;

	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	tkn_lst_printer(list_o_tokens);
	tkn_lstclear(&list_o_tokens);
	return (handle_exit_code(-1));
}

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

// Creates tokens and returns 0 if success
int	parse_input_into_token_list(t_token **list, char *input)
{
	int	pos;
	int	empty_line;

	pos = 0;
	empty_line = TRUE;
	while (input[pos])
	{
		while (ft_isspace(input[pos]))
			pos++;
		if (!input[pos])
			break ;
		if (empty_line)
		{
			empty_line = FALSE;
			handle_exit_code(0);
		}
		get_token(list, input, &pos);
	}
	return (handle_exit_code(validate_syntax(input)));
}

// Returns -1 if ok
// Returns 2 if there's a syntax error
int	validate_syntax(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((i == 0) && (str[i] == '&') && (str[i + 1] != '&'))
			return (2);
		if (i >= 1)
		{
			if ((str[i - 1] != '&') && (str[i] == '&') && (str[i + 1] != '&'))
				return (2);
			if ((str[i - 1] == '&') && (str[i] == '&') && (str[i + 1] == '&'))
				return (2);
			if ((str[i - 1] == '|') && (str[i] == '|') && (str[i + 1] == '|'))
				return (2);
		}
		i++;
	}
	return (handle_exit_code(-1));
}

void	get_token(t_token **list, char *input, int *pos)
{
	char		*tkn_str;
	t_tkn_type	token_type;

	tkn_str = NULL;
	token_type = check_type_of_token(input, pos);
	if (token_type == 0)
		isolate_word_token(input, pos, &tkn_str);
	else
		isolate_operator_token(input, pos, &tkn_str);
	if (DEBUG)
		ft_printf("--DEBUG-- Got token: %s\n", tkn_str);
	tkn_lstadd_back(list, create_token(token_type, tkn_str));
}

//	Adapted from isolate_word_token()
void	isolate_operator_token(char *input, int *pos, char **token_string)
{
	int		i;

	if ((input[*pos] == '\'') || (input[*pos] == '\"'))
	{
		handle_quoted_string(input, pos, token_string, input[*pos]);
		return ;
	}
	i = 1;
	if (((input[*pos] == '|') || (input[*pos] == '&') || (input[*pos] == '>')
			|| (input[*pos] == '<')) && (input[*pos] == input[*pos + 1]))
		i = 2;
	*token_string = ft_substr(input, *pos, i);
	*pos += i;
}

void	handle_quoted_string(char *input, int *pos, char **str, char chr)
{
	int	start;

	start = *pos;
	(*pos)++;
	while (input[*pos] && (input[*pos] != chr))
		(*pos)++;
	if (!input[*pos])
	{
		handle_exit_code(SYNTAX_ERROR);
		return ;
	}
	(*pos)++;
	*str = ft_substr(input, start, *pos - start);
}

//	char	*ft_strchr(const char *s, int c)
t_tkn_type	check_type_of_token(char *input, int *pos)
{
	if (input[*pos] == '\'')
		return (e_SINGLE_QUOTE);
	if (input[*pos] == '\"')
		return (e_DOUBLE_QUOTE);
	if (input[*pos] == '(')
		return (e_PARENTHESIS_OPEN);
	if (input[*pos] == ')')
		return (e_PARENTHESIS_CLOSE);
	if ((input[*pos] == '|') && (input[*pos + 1] != '|'))
		return (e_PIPE);
	if ((input[*pos] == '&') && (input[*pos + 1] == '&'))
		return (e_AND);
	if ((input[*pos] == '|') && (input[*pos + 1] == '|'))
		return (e_OR);
	if ((input[*pos] == '>') && (input[*pos + 1] != '>'))
		return (e_OUTPUT_REDIRECTION);
	if ((input[*pos] == '>') && (input[*pos + 1] == '>'))
		return (e_APPEND);
	if ((input[*pos] == '<') && (input[*pos + 1] != '<'))
		return (e_INPUT_REDIRECTION);
	if ((input[*pos] == '<') && (input[*pos + 1] == '<'))
		return (e_HERE_DOC);
	return (e_WORD);
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
	int		i;

	i = 0;
	while (!ft_strchr(" \\ ; \" \' ()&|><", input[*pos + i]))
		i++;
	if (i == 0)
		i = 1;
	*token_string = ft_substr(input, *pos, i);
	*pos += i;
}

t_token	*create_token(t_tkn_type token_type, char *str)
{
	t_token	*tkn;

	tkn = (t_token *)ft_malloc(sizeof(t_token) * 1);
	//if (tkn == NULL)
	//	clean_exit(); when this functions is made we can place malloc above
	tkn->type = token_type;
	tkn->token_string = str;
	tkn->next = NULL;
	return (tkn);
}
