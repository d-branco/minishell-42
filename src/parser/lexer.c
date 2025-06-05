/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/05 16:08:57 by alde-alm         ###   ########.fr       */
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

int	validate_syntax(char *str)
{
	int	i;
	int	n_parenthese;
	int	in_s_quote;
	int	in_d_quote;

	i = 0;
	in_s_quote = 0;
	in_d_quote = 0;
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
	while (str[i])
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
		i++;
	}
	if (n_parenthese != 0 || in_s_quote || in_d_quote)
		return (SYNTAX_ERROR);
	return (1);
}

void	get_token(t_token **list, char *input, int *pos)
{
	char		*part = NULL;
	char		*joined = ft_strdup("");
	char		*tmp = NULL;
	t_tkn_type	type;

	while (input[*pos])
	{
		if (ft_isspace(input[*pos]))
			break ;
		if (input[*pos] == '\'' || input[*pos] == '"')
		{
			char quote_char = input[*pos];
			type = check_type_of_token(input, pos);
			handle_quoted_string(input, pos, &part, quote_char);
		}
		else if ((type = check_type_of_token(input, pos)) == e_WORD)
			isolate_word_token(input, pos, &part);
		else
			break ;
		tmp = joined;
		joined = ft_strjoin(joined, part);
		free(tmp);
		free(part);
		part = NULL;
	}
	if (joined[0] != '\0')
		tkn_lstadd_back(list, create_token(e_WORD, joined));
	else
		free(joined);
	if (input[*pos] && !ft_isspace(input[*pos]))
	{
		t_tkn_type op_type = check_type_of_token(input, pos);
		if (op_type != e_WORD)
		{
			char *op_str = NULL;
			isolate_operator_token(input, pos, &op_str);
			tkn_lstadd_back(list, create_token(op_type, op_str));
		}
	}
}

void	handle_quoted_string(char *input, int *pos, char **str, char quote_type)
{
	int	start;

	start = *pos;
	(*pos)++;
	while (input[*pos])
	{
		if (input[*pos] == quote_type)
		{
			(*pos)++;
			break ;
		}
		(*pos)++;
	}
	if (input[*pos - 1] != quote_type)
	{
		handle_exit_code(SYNTAX_ERROR);
		return ;
	}
	*str = ft_substr(input, start, *pos - start);
}
