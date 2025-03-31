/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:10:35 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/31 14:17:20 by abessa-m         ###   ########.fr       */
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
	int		i;
	char	*str;

	str = malloc(sizeof(char));
	str[0] = '\0';
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
						write(1, "PIPE", 4);
				}
				else if (input[i + 1] == '|')
				{
					if (input[i + 2])
					{
						if ((input[i + 2] != '|') && (input[i - 1] != '|'))
						{
							if (DEBUG)
								write(1, "OR", 2);
							i++;
						}
						else
						{
							if (DEBUG)
							{
								write(2, "TESTE: ERROR DE SINTAXE\n", 24);
								return ;
							}
						}
					}
					else
					{
						if (DEBUG)
						{
							write(2, "TESTE: ERROR DE SINTAXE\n", 24);
							return ;
						}
					}
				}
			}
			if (input[i] == '&')
			{
				if (input[i + 1] == '&')
				{
					if ((input[i + 2]) && (i > 0))
					{
						if ((input[i + 2] != '&') && (input[i - 1] != '&'))
						{
							if (DEBUG)
								write(1, "AND", 3);
						}
						else
						{
							write(2, "TESTE: ERROR DE SINTAXE\n", 24);
							return ;
						}
					}
					else
					{
						write(2, "TESTE: ERROR DE SINTAXE\n", 24);
						return ;
					}
				}
			}
			if (input[i] == '\'')
			{
				if (DEBUG)
					write(1, "SINGLE_QUOTES", 13);
			}
			if (input[i] == '\"')
			{
				if (DEBUG)
					write(1, "DOUBLE_QUOTES", 13);
			}
			if (input[i] == '(')
			{
				if (DEBUG)
					write(1, "PARENTHESIS_OPEN", 16);
			}
			if (input[i] == ')')
			{
				if (DEBUG)
					write(1, "PARENTHESIS_CLOSE", 17);
			}
			if (input[i] == '<')
			{
				if (i == 0)
				{
					if (input[i + 1] != '<')
					{
						if (DEBUG)
							write(1, "REDIRECTION_INPUT", 17);
					}
				}
				else if ((input[i - 1] != '<') && (input[i + 1] != '<'))
				{
					if (DEBUG)
						write(1, "REDIRECTION_INPUT", 17);
				}
			}
			if (input[i] == '>')
			{
				if (i == 0)
				{
					if (input[i + 1] != '>')
					{
						if (DEBUG)
							write(1, "REDIRECTION_OUTPUT", 18);
					}
				}
				else if ((input[i - 1] != '>') && (input[i + 1] != '>'))
				{
					if (DEBUG)
						write(1, "REDIRECTION_OUTPUT", 18);
				}
			}
			if (input[i] == '>')
			{
				if (i == 0)
				{
					if (input[i + 1] == '>')
					{
						if (input[i + 2])
						{
							if (input[i + 2] != '>')
							{
								if (DEBUG)
									write(1, "REDIRECTION_APPEND", 18);
							}
						}
					}
				}
				else if ((input[i - 1] != '>') && (input[i + 2]))
				{
					if (input[i + 2] != '>')
					{
						if (DEBUG)
							write(1, "REDIRECTION_APPEND", 18);
					}
				}
			}
			if (input[i] == '<')
			{
				if (i == 0)
				{
					if (input[i + 1] == '<')
					{
						if (input[i + 2])
						{
							if (input[i + 2] != '<')
							{
								if (DEBUG)
									write(1, "REDIRECTION_HEREDOC", 19);
							}
						}
					}
				}
				else if ((input[i - 1] != '<') && (input[i + 2]))
				{
					if (input[i + 2] != '>')
					{
						if (DEBUG)
							write(1, "REDIRECTION_HEREDOC", 19);
					}
				}
			}
			while (ft_strchr("\\;\'\"|&()<>", input[i]) == NULL)
			{
				if (DEBUG)
					write(1, &input[i], 1);
				if (ft_strchr("\\;\'\"|&()<> ", input[i + 1]) != NULL)
					break ;
				i++;
			}
			if (DEBUG)
				write(1, " ", 1);
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
