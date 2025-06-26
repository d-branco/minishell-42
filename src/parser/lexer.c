/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/26 01:28:55 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	check_invalid_operators(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((i == 0) && (str[i] == '&' || str[i] == '|'))
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
	return (0);
}

static int	check_parentheses_and_quotes(char *str)
{
	int	i;
	int	n_parenthese;
	int	in_s_quote;
	int	in_d_quote;

	i = -1;
	n_parenthese = 0;
	in_s_quote = 0;
	in_d_quote = 0;
	while (str[++i])
	{
		if (!in_s_quote && !in_d_quote && str[i] == '(')
			n_parenthese++;
		else if (!in_s_quote && !in_d_quote && str[i] == ')')
			n_parenthese--;
		else if (!in_d_quote && str[i] == '\'')
			in_s_quote = !in_s_quote;
		else if (!in_s_quote && str[i] == '\"')
			in_d_quote = !in_d_quote;
		if (n_parenthese < 0)
			return (SYNTAX_ERROR);
	}
	if ((n_parenthese != 0) || (in_s_quote != 0) || (in_d_quote != 0))
		return (SYNTAX_ERROR);
	if (validate_heredoc_syntax(str))
		return (SYNTAX_ERROR);
	return (0);
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

int	validate_syntax(char *str)
{
	if (check_invalid_operators(str))
		return (SYNTAX_ERROR);
	if (check_parentheses_and_quotes(str))
		return (SYNTAX_ERROR);
	return (1);
}

int	handle_quoted_string(char *input, int *pos, char **str, char quote_type)
{
	int	start;

	start = *pos;
	(*pos)++;
	while (input[*pos])
	{
		if (input[*pos] == quote_type)
		{
			(*pos)++;
			*str = ft_substr(input, start, *pos - start);
			return (1);
		}
		(*pos)++;
	}
	handle_exit_code(SYNTAX_ERROR);
	*str = NULL;
	return (0);
}
