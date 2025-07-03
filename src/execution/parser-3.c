/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:35:54 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:36:08 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_redirection(t_token *tkn)
{
	return (tkn->type == e_HERE_DOC
		|| tkn->type == e_INPUT_REDIRECTION
		|| tkn->type == e_APPEND
		|| tkn->type == e_OUTPUT_REDIRECTION);
}

int	get_modifier(t_token *tkn)
{
	if (is_redirection(tkn))
		return (tkn->type);
	else
		return (-1);
}

t_ast	*make_ast_node(int type, t_ast *lhs, t_ast *rhs, t_list *pipeline)
{
	t_ast	*ret;

	ret = ft_malloc(sizeof(*ret) * 1);
	ret->type = type;
	ret->left = lhs;
	ret->right = rhs;
	ret->pipeline = pipeline;
	return (ret);
}

int	check_tkn(t_token *tok, enum e_token_type expected)
{
	if (tok->type != expected)
	{
		return (tkn_error(tok));
	}
	return (SUCCESS);
}

int	tkn_error(t_token *tok)
{
	const char	*tok_str;
	char		*prefix;
	char		*suffix;
	char		*buffer;
	int			len;

	tok_str = get_tkn_as_str(tok->type);
	prefix = "Minishell: syntax error: '";
	suffix = "'\n";
	len = ft_strlen(tok_str) + ft_strlen(prefix) + ft_strlen(suffix) + 1;
	buffer = ft_malloc(sizeof(*buffer) * len);
	ft_strlcpy(buffer, prefix, len);
	ft_strlcat(buffer, tok_str, len);
	ft_strlcat(buffer, suffix, len);
	write(2, buffer, len - 1);
	free(buffer);
	return (SYNTAX_ERROR);
}
