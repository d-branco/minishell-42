/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_interpreter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:20:04 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/03 14:22:01 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
int	handle_parse_input(char	*input)
{
	if (DEBUG)
	{
		ft_putstr_fd("--DEBUG--INPUT: ", 1);
		ft_putstr_fd(input, 1);
		write(1, "\n", 1);
	}
	ad_exit_code(0);
	if (!more_than_one_command(input))
	{
		if (check_quotes(input) || check_parenthesis(input))
		{
			write(2, "Oh! There's a syntax error.\n", 29);
			ad_exit_code(2);
		}
	}
	return (ad_exit_code(-1));
}

int	more_than_one_command(char *input)
{
	int	len;

	len = ft_strlen(input);
	if ((ft_strnstr(input, "&&", len))
		|| (ft_strnstr(input, "||", len))
		|| (ft_strnstr(input, "|", len)))
		return (printf("More than one command\n"), 1);
	return (0);
}

int	check_logic(char *input)
{
	if (!input)
		return (1);
	return (0);
}

int	check_parenthesis(char *input)
{
	int	i;
	int	sum;

	sum = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == ')')
			sum--;
		if (sum < 0)
			return (1);
		if (input[i] == '(')
			sum++;
		i++;
	}
	if (sum != 0)
		return (1);
	return (0);
}

int	check_quotes(char *input)
{
	int	i;
	int	sum;

	sum = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
			sum++;
		i++;
	}
	return (sum % 2);
}
*/
int	ad_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}
