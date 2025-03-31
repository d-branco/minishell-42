/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:07 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/31 16:09:22 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef enum e_token_type
{
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_PARENTHESIS_OPEN,
	TOKEN_PARENTHESIS_CLOSE,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OUTPUT_REDIRECTION,
	TOKEN_APPEND,
	TOKEN_INPUT_REDIRECTION,
	TOKEN_HERE_DOC,
	TOKEN_CMD_ARGS
}	t_e_token_type;

typedef struct s_token
{
	t_e_token_type	type;
	char			*token_string;
	int				length;
}	t_s_token;
