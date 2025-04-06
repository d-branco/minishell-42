/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:07 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/06 10:39:58 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef enum e_token_type
{
	e_WORD,
	e_SINGLE_QUOTE,
	e_DOUBLE_QUOTE,
	e_PARENTHESIS_OPEN,
	e_PARENTHESIS_CLOSE,
	e_PIPE,
	e_AND,
	e_OR,
	e_OUTPUT_REDIRECTION,
	e_APPEND,
	e_INPUT_REDIRECTION,
	e_HERE_DOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*token_string;
	struct s_token	*next;
}	t_token;
