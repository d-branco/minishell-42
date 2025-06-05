/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/24 19:04:43 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (handle_exit_code(-1));
}

// Returns -1 if ok
// Returns 2 if there's a syntax error
int	validate_syntax(char *str)
{
	int	i;
	int	n_parenthese;
	int	n_d_quote;
	int	n_s_quote;

	i = 0;
	while (str[i])
	{
		if ((i == 0) && (str[i] == '&') && (str[i + 1] != '&'))
			return (SYNTAX_ERROR);
		if (i >= 1)
		{
			if ((str[i - 1] != '&') && (str[i] == '&') && (str[i + 1] != '&'))
				return (SYNTAX_ERROR);
			if ((str[i - 1] == '&') && (str[i] == '&') && (str[i + 1] == '&'))
				return (SYNTAX_ERROR);
			if ((str[i - 1] == '|') && (str[i] == '|') && (str[i + 1] == '|'))
				return (SYNTAX_ERROR);
		}
		i++;
	}
	i = 0;
	n_parenthese = 0;
	n_d_quote = 0;
	n_s_quote = 0;
	while (str[i])
	{
		if (str[i] == '(')
			n_parenthese++;
		else if (str[i] == ')')
			n_parenthese--;
		else if (str[i] == '\'')
			n_s_quote++;
		else if (str[i] == '\"')
			n_d_quote++;
		i++;
		if (n_parenthese < 0)
			return (SYNTAX_ERROR);
	}
	if ((n_parenthese != 0) || ((n_s_quote % 2) != 0) || ((n_d_quote % 2) != 0))
		return (SYNTAX_ERROR);
	if (validate_heredoc_syntax(str))
		return (SYNTAX_ERROR);
	return (1);
}

int	validate_heredoc_syntax(char *input)
{
	int	i;
	int	j;

	i = -1;
	while (input[++i])
	{
		if ((input[i] == '<') && (input[i + 1]) && (input[i + 1] == '<'))
		{
			j = i + 1;
			while (input[++j])
			{
				if (!(input[j])
					|| (input[j] == '&' && input[j - 1] == '&')
					|| (input[j] == '|' && input[j - 1] == '|'))
					break ;
				if ((input[j] == '<')
					&& (input[j + 1]) && (input[j + 1] == '<'))
					return (SYNTAX_ERROR);
			}
		}

	}
	return (FALSE);
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
