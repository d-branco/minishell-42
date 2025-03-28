/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/28 10:16:40 by abessa-m         ###   ########.fr       */
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

static int	check_syntax_for_semicolon_backslash(char *input);

int	lexer(char *input)
{
	//t_node	*head_node;

	//head_node = NULL;
	if (!input)
		return (-1);
	else if (check_syntax_for_semicolon_backslash(input))
		return (SYNTAX_ERROR);
	return (0);
}

static int	check_syntax_for_semicolon_backslash(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' || input[i] == '\\')
			return (SYNTAX_ERROR);
		i++;
	}
	return (0);
}
