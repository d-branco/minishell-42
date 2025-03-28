/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/28 19:21:36 by abessa-m         ###   ########.fr       */
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

int	lexer(char *input, t_l_no *head_node)
{
	if (!input)
		return (-1);
	else if (check_syntax_for_semicolon_backslash(input))
		return (SYNTAX_ERROR);
	looping_lexer(input, head_node);
	return (0);
}

void	looping_lexer(char *input, t_l_no *head_node)
{
	int	i;

	i = 0;
	if (DEBUG)
		write(1, "--DEBUG--lexer: ", 16);
	while (input[i])
	{
		if (ft_isprint(input[i]) && !ft_isspace(input[i]))
		{
			if (input[i] == '|')
			{
				if ((i == 0) || (!input[i + 1]))
				{
					write(2, "TESTE: ERROR DE SINTAXE\n", 24);
					return ;
				}
				if (input[i - 1] != '|' && input[i + 1] && input[i + 1] != '|')
				{
					if (DEBUG)
						write(1, "PIPE ", 5);
				}
				else if (input[i + 1] == '|')
				{
					if (input[i + 2])
					{
						if ((input[i + 2] != '|') && (input[i - 1] != '|'))
						{
							if (DEBUG)
								write(1, "OR ", 3);
							i++;
						}
						else
							if (DEBUG)
							{
								write(2, "TESTE: ERROR DE SINTAXE\n", 24);
								return ;
							}
					}
					else
						if (DEBUG)
						{
							write(2, "TESTE: ERROR DE SINTAXE\n", 24);
							return ;
						}
				}
			}
			if (input[i] == '&')
			{
				if (input[i + 1] == '&')
				{
					if (input[i + 2])
					{
						if (input[i + 2] != '&')
						{
							if (DEBUG)
								write(1, "AND ", 4);
						}
					}
				}
			}
			//QUOTE (single, double)?
			//PARANTHESIS?
			//REDIRECTION(<, <<, >, >>)?
			//ARGUMENTS(separeted by spaces)
		}
		i++;
	}
	if (DEBUG)
		write(1, "\n", 1);
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
