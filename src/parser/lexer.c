/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/28 17:05:36 by abessa-m         ###   ########.fr       */
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
static int	check_start_char(char *input);

int	lexer(char *input, t_l_no *head_node)
{
	if (!input)
		return (-1);
	else if (check_syntax_for_semicolon_backslash(input)
		|| check_start_char(input))
		return (SYNTAX_ERROR);
	looping_lexer(input, head_node);
	return (0);
}

void	looping_lexer(char *input, t_l_no *head_node)
{
	int	i;

	i = 0;
	if (!ft_isspace(input[i]))
		printf("Nao comeca com espaco!\n");
	while (input[i])
	{
		i++;
	}
}

static int	check_start_char(char *input)
{
	if (input[0] == '|' || input[0] == '&')
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
