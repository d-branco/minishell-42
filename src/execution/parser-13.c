/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-13.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:16:05 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:16:16 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_ast(t_ast **ast, t_token **tkn)
{
	*ast = NULL;
	if ((*tkn)->type == e_END)
		return (SUCCESS);
	if (parse_tokens(ast, tkn) != SUCCESS)
		return (SYNTAX_ERROR);
	if ((*tkn)->type != e_END)
	{
		ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int	parse_tokens(t_ast **ast, t_token **tkn)
{
	t_ast	*ptr;
	int		tkn_type;

	*ast = NULL;
	if (parse_conditionnal(ast, tkn) != SUCCESS)
		return (SYNTAX_ERROR);
	while (TRUE)
	{
		if ((*tkn)->type == e_END || (*tkn)->type == e_PARENTHESIS_CLOSE)
			break ;
		if ((*tkn)->type != e_AND && (*tkn)->type != e_OR)
		{
			ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
			return (SYNTAX_ERROR);
		}
		tkn_type = (*tkn)->type;
		next_token(tkn);
		if (parse_conditionnal(&ptr, tkn) != SUCCESS)
		{
			free_ast(ptr);
			return (SYNTAX_ERROR);
		}
		*ast = make_ast_node(tkn_type, *ast, ptr, NULL);
	}
	return (SUCCESS);
}

int	parse_conditionnal(t_ast **ast, t_token **tkn)
{
	t_list	*pipeline;

	*ast = NULL;
	if ((*tkn)->type == e_PARENTHESIS_OPEN)
	{
		next_token(tkn);
		if (parse_tokens(ast, tkn) != SUCCESS)
			return (SYNTAX_ERROR);
		if (check_tkn(*tkn, e_PARENTHESIS_CLOSE) != SUCCESS)
			return (SYNTAX_ERROR);
		next_token(tkn);
	}
	else
	{
		if (parse_pipeline(&pipeline, tkn) != SUCCESS)
		{
			free_pipeline(pipeline);
			return (SYNTAX_ERROR);
		}
		*ast = make_ast_node(e_PIPE, NULL, NULL, pipeline);
	}
	return (SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
int	handle_quote(char *c, t_quote_state *state)
{
	int	ret;

	ret = 1;
	if (*c == '\'' && !state->double_quote && !state->escaped)
		state->single_quote = !state->single_quote;
	else if (*c == '"' && !state->single_quote && !state->escaped)
		state->double_quote = !state->double_quote;
	else if (*c == '\\' && !state->single_quote && !state->escaped
		&& needs_escaping(*(c + 1), state))
		state->escaped = TRUE;
	else
	{
		ret = 0;
		state->escaped = FALSE;
	}
	return (ret);
}

int	needs_escaping(char c, t_quote_state *state)
{
	return (c == '\\' || c == '"' || (c == '\'' && !state->double_quote)
		|| (c == '$') || (c == '*' && !state->double_quote));
}
