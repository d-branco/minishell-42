/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:07 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/17 09:30:14 by abessa-m         ###   ########.fr       */
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
}	t_tkn_type;

typedef struct s_token
{
	t_tkn_type		type;
	char			*token_string;
	struct s_token	*next;
}	t_token;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIRECT,
	AST_AND,
	AST_OR
}	t_ast_type;

typedef struct s_ast_node
{
	t_ast_type			type;
	void				*content;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

//	char				**args;				Splitted array (NULL-terminated)
typedef struct s_command
{
	char				*command;
	char				**args;
}	t_command;

//	char				*file;				Filename or heredoc delimiter
typedef struct s_redirect
{
	t_tkn_type			redirect_type;
	char				*file;
}	t_redirect;

//parser/ast.c
t_ast_node	*build_ast(t_token **tokens);
t_ast_node	*parse_logical_ops(t_token **tokens);
t_ast_node	*parse_pipe(t_token **tokens);
t_ast_node	*parse_redirections(t_token **tokens);
t_ast_node	*create_ast_node(t_ast_type type, void *content);
//parser/ast-print.c
void		print_ast(t_ast_node *node, int depth);
//parser/ast-free.c
void		free_arg_list(t_list *arg_list);
void		free_arg_list_structure(t_list *arg_list);
void		free_ast_node(t_ast_node *node);
//parse/ast-parse-cmd.c
t_ast_node	*parse_commands(t_token **tokens);
//parse/ast-parse-cmd2.c
t_ast_node	*handle_tokens_inside_parenthesis(t_token **tokens);
int			is_valid_token_for_argument(t_token *token);

//parser/parser.c
int			parser(char *input);
//parser/lexer.c
int			parse_input_into_token_list(t_token **list, char *input);
int			validate_syntax(char *str);
void		get_token(t_token **list, char *input, int *pos);
void		handle_quoted_string(char *input, int *pos, char **str, char chr);
//parser/lexer-list.c
void		tkn_lst_printer(t_token *lst);
void		tkn_lstclear(t_token **lst);
t_token		*create_token(t_tkn_type token_type, char *token_string);
void		tkn_lstadd_back(t_token **lst, t_token *new);
//parser/lexer-tokenizer.c
t_tkn_type	check_type_of_token(char *input, int *pos);
void		isolate_word_token(char *input, int *pos, char **token_string);
void		isolate_operator_token(char *input, int *pos, char **token_string);
