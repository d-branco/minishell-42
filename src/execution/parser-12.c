/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-12.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:15:09 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:15:24 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (!ast->right && !ast->left)
	{
		free_pipeline(ast->pipeline);
		free(ast);
		return ;
	}
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast);
}

void	free_pipeline(t_list *pipeline)
{
	ft_lstclear(&pipeline, (void (*)(void *))free_tube_lst);
}

void	free_tube_lst(t_tube *lst)
{
	t_tube	*previous;

	previous = 0;
	while (lst)
	{
		if (previous)
			free(previous->word);
		free(previous);
		previous = lst;
		lst = lst->next;
	}
	if (previous)
		free(previous->word);
	free(previous);
}

////////////////////////////////////////////////////////////////////////////////
int	exec_ast(t_ast *ast, t_env **env, int previous)
{
	int	error_code;

	if (!ast)
		return (0);
	if (!ast->left && !ast->right)
	{
		if (pipeline_expansion(&ast->pipeline, *env, previous) != 0)
			return (1);
		return (exec_pipeline(ast->pipeline, env, previous));
	}
	error_code = exec_ast(ast->left, env, previous);
	if (!(!error_code == (ast->type == e_OR)))
		error_code = exec_ast(ast->right, env, previous);
	return (error_code);
}

int	exec_pipeline(t_list *pipeline, t_env **env, int previous)
{
	t_exec	*exec;
	int		error_code;

	exec = init_pipeline(pipeline, env, previous);
	error_code = 0;
	if (exec->n == 1 && exec->cmds[0].builtin != -1)
		error_code = fd_builtin(exec, 0);
	else if (exec->n >= 1)
		error_code = cmds_with_forks(exec);
	free_exec(exec);
	free(exec);
	return (error_code);
}
