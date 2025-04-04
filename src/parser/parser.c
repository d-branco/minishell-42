/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/04 15:05:04 by abessa-m         ###   ########.fr       */
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

int	parser(char *input)
{
	t_token	*list_o_tokens;

	parse_input_into_token_list(&list_o_tokens, input);
	//print list_o_tokens
	return (0);// temp
}

////typedef struct s_token
////{
////	t_token_type	type;
////	char			*token_string;
////	int				length;
////}	t_token;

// Creates a list of tokens and returns 0 if success
int	parse_input_into_token_list(t_token **list, char *input)
{
	int	pos;

	pos = 0;
	get_token(list, input, &pos);
	return (0); //temp
}

void	get_token(t_token **list, char *input, int *pos)
{
	char	*token_string;

	while (ft_isspace(input[*pos]))
		(*pos)++;
	//check the type of token
	//if string
	isolate_word_token(input, pos, &token_string);
	//else
	//isolate_operator_token();
	//then
	//add token to list
	if (DEBUG)
		ft_printf("First token: %s\n", token_string);
	if (token_string)
		free(token_string);
}

//	char	*ft_substr(char const *s, unsigned int start, size_t len);
void	isolate_word_token(char *input, int *pos, char **token_string)
{
	int	i;

	i = 0;
	while (input[*pos + i] && !ft_isspace(input[*pos + i])
		&& input[*pos + i] != '\'' && input[*pos + i] != '\"')
	{
		i++;
	}
	*token_string = ft_substr(input, *pos, i);
}
