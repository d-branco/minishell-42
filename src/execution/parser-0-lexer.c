/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:03:58 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:04:13 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	lexer(t_token **tkn, char **str)
{
	int		error_code;
	char	*word;
	int		tkn_type;

	error_code = 0;
	while (ft_isspace(**str))
		(*str)++;
	tkn_type = get_tkn_type(*str);
	if (tkn_type != e_WORD)
	{
		*tkn = make_one_tkn(NULL, NULL, tkn_type);
		if (tkn_type != e_END)
			*str += ft_strlen(get_tkn_as_str(tkn_type));
	}
	else
	{
		error_code = get_str_token(&word, str);
		*tkn = make_one_tkn(NULL, word, e_WORD);
	}
	return (error_code);
}

int	get_tkn_type(char *str)
{
	int			i;
	const char	*tkn_str;

	if (*str == '\0' || *str == '\n')
		return (e_END);
	i = 0;
	while (i < 9)
	{
		tkn_str = get_tkn_as_str(i);
		if (!ft_strncmp(str, tkn_str, ft_strlen(tkn_str)))
			return (i);
		i++;
	}
	return (e_WORD);
}

const char	*get_tkn_as_str(enum e_token_type n)
{
	static const char	*tkn_strings[9] = {
		"(", ")", "&&", "||", "<<", "<", ">>", ">", "|"};

	if (n == e_END)
		return ("EOL");
	if (n > 9 || n < 0)
		return (NULL);
	return (tkn_strings[n]);
}

t_token	*make_one_tkn(t_token *next, char *str, enum e_token_type type)
{
	t_token	*new;

	new = (t_token *)ft_malloc(sizeof(t_token) * 1);
	new->next = next;
	new->str = str;
	new->type = type;
	return (new);
}

int	get_str_token(char **word, char **str)
{
	int				i;
	t_quote_state	state;

	state = (t_quote_state){0, 0, 0};
	i = 0;
	while ((*str)[i] && (!ft_isspace((*str)[i])
		|| state.escaped || state.single_quote || state.double_quote))
	{
		handle_quote(*str + i, &state);
		if (!state.escaped && !state.single_quote && !state.double_quote
			&& get_tkn_type(*str + i) != e_WORD)
			break ;
		i++;
	}
	*word = ft_substr(*str, 0, i);
	*str += i;
	if (state.single_quote || state.double_quote)
	{
		ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
		return (handle_exit_code(SYNTAX_ERROR));
	}
	return (0);
}
