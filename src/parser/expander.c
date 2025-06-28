/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:02:22 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/26 15:58:43 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	split_and_extend(char *arg, char ***new_args)
{
	char	**split;
	int		k;

	split = ft_split(arg, ' ');
	if (!split)
		return ;
	k = 0;
	while (split[k])
	{
		ft_strarr_extend(new_args, (char *[]){ft_strdup(split[k]), NULL});
		k++;
	}
	ft_strarr_free(split);
}

static void	expand_and_append(char **expanded, char ***new_args, int was_quoted)
{
	int		j;
	int		is_assignment;

	j = 0;
	while (expanded[j])
	{
		is_assignment = ft_strchr(expanded[0], '=') != NULL;
		if (!was_quoted && !is_assignment && expanded[1] == NULL
			&& ft_strchr(expanded[0], ' '))
			split_and_extend(expanded[0], new_args);
		else
			ft_strarr_extend(new_args,
				(char *[]){expanded[j], NULL});
		j++;
	}
}

void	expand_arguments(t_command *cmd, t_mnsh *shell)
{
	int		i;
	int		was_quoted;
	char	**expanded;
	char	**new_args;

	i = 0;
	new_args = NULL;
	while (cmd->args && cmd->args[i])
	{
		expanded = expand_argument_and_wildcard(cmd->args[i],
				shell, &was_quoted);
		if (expanded)
		{
			expand_and_append(expanded, &new_args, was_quoted);
			ft_strarr_free(expanded);
		}
		i++;
	}
	ft_strarr_free(cmd->args);
	cmd->args = new_args;
	cmd->argc = ft_strarr_len(cmd->args);
}
