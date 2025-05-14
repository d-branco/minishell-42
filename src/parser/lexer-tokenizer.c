/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:22:15 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/07 12:43:18 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//	char	*ft_strchr(const char *s, int c)
t_tkn_type	check_type_of_token(char *input, int *pos)
{
	if (input[*pos] == '\'')
		return (e_SINGLE_QUOTE);
	if (input[*pos] == '\"')
		return (e_DOUBLE_QUOTE);
	if (input[*pos] == '(')
		return (e_PARENTHESIS_OPEN);
	if (input[*pos] == ')')
		return (e_PARENTHESIS_CLOSE);
	if ((input[*pos] == '|') && (input[*pos + 1] != '|'))
		return (e_PIPE);
	if ((input[*pos] == '&') && (input[*pos + 1] == '&'))
		return (e_AND);
	if ((input[*pos] == '|') && (input[*pos + 1] == '|'))
		return (e_OR);
	if ((input[*pos] == '>') && (input[*pos + 1] != '>'))
		return (e_OUTPUT_REDIRECTION);
	if ((input[*pos] == '>') && (input[*pos + 1] == '>'))
		return (e_APPEND);
	if ((input[*pos] == '<') && (input[*pos + 1] != '<'))
		return (e_INPUT_REDIRECTION);
	if ((input[*pos] == '<') && (input[*pos + 1] == '<'))
		return (e_HERE_DOC);
	return (e_WORD);
}

//	char	*ft_substr(char const *s, unsigned int start, size_t len);
void	isolate_word_token(char *input, int *pos, char **token_string)
{
	int		i;

	i = 0;
	while (!ft_strchr(" \\ ; \" \' ()&|><", input[*pos + i]))
		i++;
	if (i == 0)
		i = 1;
	*token_string = ft_substr(input, *pos, i);
	*pos += i;
}

//	Adapted from isolate_word_token()
void	isolate_operator_token(char *input, int *pos, char **token_string)
{
	int		i;

	if ((input[*pos] == '\'') || (input[*pos] == '\"'))
	{
		handle_quoted_string(input, pos, token_string, input[*pos]);
		return ;
	}
	i = 1;
	if (((input[*pos] == '|') || (input[*pos] == '&') || (input[*pos] == '>')
			|| (input[*pos] == '<')) && (input[*pos] == input[*pos + 1]))
		i = 2;
	*token_string = ft_substr(input, *pos, i);
	*pos += i;
}
