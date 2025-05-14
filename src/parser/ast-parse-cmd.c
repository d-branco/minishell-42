/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast-parse-cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:11:54 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list		*build_argument_list(t_token **tokens, int *arg_count);
static t_command	*create_command_structure(t_list *arg_list, int arg_count);
static void			cleanup_command_on_failure(t_command *cmd, int arg_count);
static t_ast_node	*finalize_ast_node(t_command *cmd, int arg_count);

t_ast_node	*parse_commands(t_token **tokens)
{
	t_list		*arg_list;
	t_command	*cmd;
	t_ast_node	*node;
	int			arg_count;

	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == e_PARENTHESIS_OPEN)
		return (handle_tokens_inside_parenthesis(tokens));
	if (!is_valid_token_for_argument(*tokens))
		return (NULL);
	arg_list = build_argument_list(tokens, &arg_count);
	if (!arg_list)
		return (NULL);
	if (arg_count == 0)
		return (NULL);
	cmd = create_command_structure(arg_list, arg_count);
	free_arg_list_structure(arg_list);
	if (!cmd)
		return (NULL);
	node = finalize_ast_node(cmd, arg_count);
	return (node);
}

static t_list	*build_argument_list(t_token **tokens, int *arg_count)
{
	t_list	*arg_list;
	t_token	*curr;
	char	*arg;

	arg_list = NULL;
	curr = *tokens;
	*arg_count = 0;
	while (is_valid_token_for_argument(curr))
	{
		arg = strdup(curr->token_string);
		if (!arg)
		{
			free_arg_list(arg_list);
			return (NULL);
		}
		ft_lstadd_back(&arg_list, ft_lstnew(arg));
		(*arg_count)++;
		curr = curr->next;
	}
	*tokens = curr;
	return (arg_list);
}

static t_command	*create_command_structure(t_list *arg_list, int arg_count)
{
	t_command	*cmd;
	t_list		*arg_node;
	int			i;

	cmd = malloc(sizeof(t_command));
	arg_node = arg_list;
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	while (i < arg_count)
	{
		cmd->args[i] = arg_node->content;
		arg_node = arg_node->next;
		i++;
	}
	cmd->args[arg_count] = NULL;
	cmd->command = cmd->args[0];
	cmd->argc = arg_count;
	return (cmd);
}

static void	cleanup_command_on_failure(t_command *cmd, int arg_count)
{
	int	i;

	i = 0;
	while (i < arg_count)
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	free(cmd);
}

static t_ast_node	*finalize_ast_node(t_command *cmd, int arg_count)
{
	t_ast_node	*node;

	node = create_ast_node(AST_COMMAND, cmd);
	if (!node)
	{
		cleanup_command_on_failure(cmd, arg_count);
		return (NULL);
	}
	return (node);
}
