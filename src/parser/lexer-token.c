/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer-token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:46:37 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/06 09:56:58 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_word_part(char **joined, char **part)
{
	char	*tmp;

	tmp = *joined;
	*joined = ft_strjoin(*joined, *part);
	free(tmp);
	free(*part);
	*part = NULL;
}

static void	handle_operator(t_token **list, char *input, int *pos)
{
	t_tkn_type	op_type;
	char		*op_str;

	op_type = check_type_of_token(input, pos);
	if (op_type != e_WORD)
	{
		op_str = NULL;
		isolate_operator_token(input, pos, &op_str);
		tkn_lstadd_back(list, create_token(op_type, op_str));
	}
}

static int	handle_token_segment(char *input, int *pos, char **part)
{
	t_tkn_type	type;
	char		quote_char;

	if (input[*pos] == '\'' || input[*pos] == '"')
	{
		quote_char = input[*pos];
		type = check_type_of_token(input, pos);
		handle_quoted_string(input, pos, part, quote_char);
	}
	else
	{
		type = check_type_of_token(input, pos);
		if (type == e_WORD)
			isolate_word_token(input, pos, part);
		else
			return (0);
	}
	return (1);
}

static void	read_word(t_token **list, char *input, int *pos, char **joined)
{
	char	*part;
	int		ok;

	part = NULL;
	while (input[*pos])
	{
		if (ft_isspace(input[*pos]))
			break ;
		ok = handle_token_segment(input, pos, &part);
		if (!ok)
			break ;
		handle_word_part(joined, &part);
	}
	if ((*joined)[0] != '\0')
		tkn_lstadd_back(list, create_token(e_WORD, *joined));
	else
		free(*joined);
}

void	get_token(t_token **list, char *input, int *pos)
{
	char	*joined;

	joined = ft_strdup("");
	read_word(list, input, pos, &joined);
	if (input[*pos] && !ft_isspace(input[*pos]))
		handle_operator(list, input, pos);
}
