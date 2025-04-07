/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:57:29 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/07 18:26:55 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	AST_COMMAND,		Simple command with arguments
//	AST_PIPE,			Pipe between commands
//	AST_REDIRECT,		Redirection (>, <, >>, <<)
//	AST_AND,			Logical AND (&&)
//	AST_OR				Logical OR (||)
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIRECT,
	AST_AND,
	AST_OR
}	t_ast_type;

//	void					*content;			Pointer to specific struct
//	struct s_ast_node		*left;				Left child
//	struct s_ast_node		*right;				Right child
typedef struct s_ast_node
{
	t_ast_type			type;
	void				*content;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

// Content structures for different node types
//	char					*command;			The command itself
//	char					**args;				Splitted array (NULL-terminated)
typedef struct s_command
{
	char				*command;
	char				**args;
}	t_command;

//	t_tkn_type			redirect_type;		e_OUTPUT_REDIRECTION, e_APPEND, etc.
//	char					*file;				Filename or heredoc delimiter
typedef struct s_redirect
{
	t_tkn_type			redirect_type;
	char				*file;
}	t_redirect;