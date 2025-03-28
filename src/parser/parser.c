/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/28 08:22:44 by abessa-m         ###   ########.fr       */
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

//static int	check_logic_operator_existence(char *input);
//static int	print_lexer(char *input);

void	parser(char *input)
{
	int	error_number;

	if (DEBUG)
		printf("--DEBUG--input to parse: %s\n", input);
	error_number = lexer(input);
	if (error_number == 2)
		printf("SYNTAX ERROR, oh my!\n");
}

/*
static int	print_lexer(char *input)
{
	int	i;

	i = 0;
	if (!input[i])
		return (-1);
	else
	{
		while (input[i])
		{
			if (input[i] == ';' || input[i] == '\\')
				return (SYNTAX_ERROR);
			i++;
		}
		i = 0;
		if (input[i] == '|' || input[i] == '&')
			return (SYNTAX_ERROR);
		if (input[i] != ' ')
			write(1, &input[i], 1);
	}
	i = 1;
	while (input[i])
	{
		if (ft_isprint(input[i]) && input[i] != ' ')
		{
			if ((input[i] == '|') && (input[i + 1] != '|'))
			{
				if (input[i - 1] != ' ')
					write(1, " ", 1);
				write(1, "PIPE", 4);
				if (input[i + 1] != ' ')
					write(1, " ", 1);
			}
			else if ((input[i] == '|') && (input[i + 1] == '|'))
			{
				if (input[i + 2])
				{
					if (input[i + 2] == '|')
						return (SYNTAX_ERROR);
					if (input[i - 1] != ' ')
						write(1, " ", 1);
					write(1, "OR", 3);
					if (input[i + 2] != ' ')
						write(1, " ", 1);
					i++;
				}
				else
					return (SYNTAX_ERROR);
			}
			else if ((input[i] == '&') && (input[i + 1] == '&'))
			{
				if (input[i + 2])
				{
					if (input[i + 2] == '&')
						return (SYNTAX_ERROR);
					if (input[i - 1] != ' ')
						write(1, " ", 1);
					write(1, "AND", 3);
					if (input[i + 2] != ' ')
						write(1, " ", 1);
					i++;
				}
				else
					return (SYNTAX_ERROR);
			}
			else if ((input[i] == '\''))
			{
				if (input[i - 1] != ' ')
					write(1, " ", 1);
				write(1, "SINGLE_QUOTE", 12);
				i++;
				while (input[i] != '\'')
				{
					if (!input[i])
						return (SYNTAX_ERROR);
					write(1, &input[i], 1);
					i++;
				}
				write(1, "SINGLE_QUOTE", 12);
				if (input[i + 1] != ' ')
					write(1, " ", 1);
			}
			else if ((input[i] == '\"'))
			{
				if (input[i - 1] != ' ')
					write(1, " ", 1);
				write(1, "DOUBLE_QUOTE", 12);
				i++;
				while (input[i] != '\"')
				{
					if (!input[i])
						return (SYNTAX_ERROR);
					write(1, &input[i], 1);
					i++;
				}
				write(1, "DOUBLE_QUOTE", 12);
				if (input[i + 1] != ' ')
					write(1, " ", 1);
			}
			else
				write(1, &input[i], 1);
		}
		else
		{
			if (ft_isprint(input[i - 1]) && (input[i - 1] != ' '))
			{
				write(1, " ", 1);
			}
		}
		i++;
	}
	write(1, "\n", 1);
	return (0);
}

static int	check_logic_operator_existence(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if ((input[i] == '&' && input[i + 1] == '&')
			|| (input[i] == '|' && input[i + 1] == '|'))
			return (TRUE);
		i++;
	}
	return (FALSE);
}*/
