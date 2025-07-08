/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_n_etc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:08:51 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:09:22 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

void	handle_args(int argc, char **argv)
{
	if (argc > 1)
	{
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		{
			ft_putstr_fd("Minishell: try it with bash!\n", STDERR_FILENO);
			exit (SYNTAX_ERROR);
		}
		else
		{
			ft_putstr_fd("Too many arguments, dear ;)\n", STDERR_FILENO);
			exit (SYNTAX_ERROR);
		}
	}
}

bool	ft_check_input(const char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (true);
		input++;
	}
	return (false);
}

void	free_shell(t_mnsh *shell)
{
	int	i;

	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
			free(shell->envp[i++]);
		free (shell->envp);
	}
	free (shell);
}

void	init_shell(t_mnsh *shell, char **envp)
{
	shell->envp = init_envp(envp);
	handle_shlvl(shell);
}
