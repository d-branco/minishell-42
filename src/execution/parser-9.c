/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-9.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:11:45 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:12:00 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**extract_args(t_tube *lst)
{
	t_tube	*current;
	char	**args;
	int		i;

	i = 0;
	current = lst;
	while (current)
	{
		if (current->modifier == -1)
			i++;
		current = current->next;
	}
	args = ft_malloc(sizeof(*args) * (i + 1));
	current = lst;
	i = 0;
	while (current)
	{
		if (current->modifier == -1)
			args[i++] = ft_strdup(current->word);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

void	init_cmd_path(t_cmd *cmd, t_exec *exec)
{
	if (!cmd->args[0])
		return ;
	if (ret_builtin_enum(cmd->args[0]) != -1)
		cmd->builtin = ret_builtin_enum(cmd->args[0]);
	else
		cmd->status = find_cmd(exec->path, cmd->args[0], &(cmd->full_path));
}

int	ret_builtin_enum(char *str)
{
	int		i;

	i = -1;
	while (++i < 7)
	{
		if (!ft_strcmp(str, ret_builtin_literal(i)))
			return (i);
	}
	return (-1);
}

const char	*ret_builtin_literal(int n)
{
	static const char	*tkn_str[7] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"};

	if (n > 7 || n < 0)
		return (NULL);
	return (tkn_str[n]);
}

void	heredoc_failure(void)
{
	char	*args[4];

	args[0] = "sh";
	args[1] = "-c";
	args[2] = "exit 130";
	args[3] = NULL;
	execve("/usr/bin/sh", args, NULL);
}
