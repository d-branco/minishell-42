/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:31:45 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:32:48 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_pipeline(t_list **pipeline, t_token **tok)
{
	*pipeline = NULL;
	if ((*tok)->type == e_END)
	{
		tkn_error(*tok);
		return (SYNTAX_ERROR);
	}
	if (make_pipeline(pipeline, tok) == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	return (SUCCESS);
}

int	make_pipeline(t_list **pipeline, t_token **tok)
{
	t_tube	*cmd;
	int		ret;

	ret = parse_cmd(&cmd, tok);
	while (ret == 0)
	{
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		if ((*tok)->type == e_END || (*tok)->type == e_AND
			|| (*tok)->type == e_OR || (*tok)->type == e_PARENTHESIS_CLOSE)
			break ;
		if (check_tkn(*tok, e_PIPE) != SUCCESS)
		{
			ret = SYNTAX_ERROR;
			break ;
		}
		next_tkn(tok);
		ret = parse_cmd(&cmd, tok);
	}
	if (ret == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	if (!*pipeline)
		return (tkn_error(*tok));
	return (SUCCESS);
}

void	next_tkn(t_token **list)
{
	*list = (*list)->next;
}

int	parse_cmd(t_tube **cmd, t_token **tok)
{
	t_tube	**current;
	int		i;
	int		ret;

	*cmd = NULL;
	current = cmd;
	i = 0;
	ret = parse_tube(current, tok);
	while (ret == 0)
	{
		current = &(*current)->next;
		ret = parse_tube(current, tok);
		i++;
	}
	if (ret == SYNTAX_ERROR)
	{
		return (ret);
	}
	if (i == 0)
	{
		return (tkn_error(*tok));
	}
	return (0);
}

int	parse_tube(t_tube **tube, t_token **tok)
{
	int		modifier;

	*tube = NULL;
	if ((*tok)->type == e_END)
		return (-1);
	if (is_redirection(*tok))
	{
		modifier = get_modifier(*tok);
		next_tkn(tok);
		if (check_tkn(*tok, e_WORD) != 0)
			return (SYNTAX_ERROR);
		*tube = make_tube(&(t_tube){(*tok)->str, modifier, 0, NULL});
		next_tkn(tok);
	}
	else if ((*tok)->type == e_WORD)
	{
		*tube = make_tube(
				&(t_tube){(*tok)->str, -1, 0, NULL});
		next_tkn(tok);
	}
	else
		return (-1);
	return (SUCCESS);
}
