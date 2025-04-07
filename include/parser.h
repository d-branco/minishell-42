/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:07 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/07 12:35:59 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	'"()|&><
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
}	t_tkn_type;

typedef struct s_token
{
	t_tkn_type		type;
	char			*token_string;
	struct s_token	*next;
}	t_token;

//parser/parser.c
int			parser(char *input);
int			parse_input_into_token_list(t_token **list, char *input);
void		get_token(t_token **list, char *input, int *pos);
void		isolate_word_token(char *input, int *pos, char **token_string);
t_token		*create_token(t_tkn_type token_type, char *token_string);
void		del_content(void *content);
void		tkn_lstadd_back(t_token **lst, t_token *new);
void		tkn_lstclear(t_token **lst);
void		tkn_lst_printer(t_token *lst);
t_tkn_type	check_type_of_token(char *input, int *pos);
void		isolate_operator_token(char *input, int *pos, char **token_string);
void		handle_quoted_string(char *input, int *pos, char **str, char chr);
int			validate_syntax(char *str);