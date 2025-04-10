/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:57:29 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/08 18:10:40 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIRECT,
	AST_AND,
	AST_OR
}	t_ast_type;

//	void				*content;			Pointer to specific struct
//	struct s_ast_node	*left;				Left child
//	struct s_ast_node	*right;				Right child
typedef struct s_ast_node
{
	t_ast_type			type;
	void				*content;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

//	char				*command;
//	char				**args;				Splitted array (NULL-terminated)
typedef struct s_command
{
	char				*command;
	char				**args;
}	t_command;

//	t_tkn_type			redirect_type;		e_OUTPUT_REDIRECTION, e_APPEND, etc.
//	char				*file;				Filename or heredoc delimiter
typedef struct s_redirect
{
	t_tkn_type			redirect_type;
	char				*file;
}	t_redirect;

t_ast_node		*build_ast(t_token **tokens);
t_ast_node		*parse_commands(t_token **tokens);
t_ast_node		*parse_logical_ops(t_token **tokens);
t_ast_node		*parse_pipe(t_token **tokens);
t_ast_node		*parse_redirections(t_token **tokens);
void			free_arg_list(t_list *arg_list);
void			free_arg_list_structure(t_list *arg_list);
void			free_ast_node(t_ast_node *node);
t_ast_node		*create_ast_node(t_ast_type type, void *content);
void			print_ast(t_ast_node *node, int depth);