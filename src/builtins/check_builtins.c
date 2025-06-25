/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:24:23 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/17 13:42:45 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(t_command *cmd)
{
	const char	*builtins[8];
	int			i;

	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "env";
	builtins[3] = "exit";
	builtins[4] = "export";
	builtins[5] = "pwd";
	builtins[6] = "unset";
	builtins[7] = NULL;
	i = -1;
	while (++i < 7)
	{
		if (ft_strlen(builtins[i]) == ft_strlen(cmd->command))
		{
			if (ft_strncmp(builtins[i], cmd->command,
					ft_strlen(cmd->command)) == 0)
				return (1);
		}
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_mnsh *shell)
{
	if (strncmp(cmd->command, "cd", ft_strlen(cmd->command)) == 0)
		return (ft_cd(cmd->argc, cmd->args, shell));
	else if (ft_strncmp(cmd->command, "echo", ft_strlen(cmd->command)) == 0)
		return (ft_echo(cmd->args));
	else if (ft_strncmp(cmd->command, "env", ft_strlen(cmd->command)) == 0)
		return (ft_env(cmd->args, shell->envp));
	else if (ft_strncmp(cmd->command, "exit", ft_strlen(cmd->command)) == 0)
		return (ft_exit(cmd->argc, cmd->args, shell));
	else if (ft_strncmp(cmd->command, "export", ft_strlen(cmd->command)) == 0)
		return (ft_export(cmd->args, shell));
	else if (ft_strncmp(cmd->command, "pwd", ft_strlen(cmd->command)) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->command, "unset", ft_strlen(cmd->command)) == 0)
		return (ft_unset(cmd->args, &shell->envp));
	return (0);
}

void	ft_free_env(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	*ft_getenv(char **envp, char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, len) == 0 && (*envp)[len] == '=')
			return (ft_strdup(*envp + len + 1));
		envp++;
	}
	return (NULL);
}

static int	envp_size(char **envp)
{
	int	i = 0;

	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}


int	export_var(const char *av, char ***envp)
{
	char	*var_name;
	char	*value;
	char	*equal_posit;
	char	*plus_equal;
	char	*existing;
	char	*new_val;
	char	*key;

	plus_equal = ft_strnstr(av, "+=", ft_strlen(av));
	if (plus_equal)
	{
		var_name = ft_substr(av, 0, plus_equal - av);
		value = ft_strdup(plus_equal + 2);
		if (!var_name || !value)
			return (free(var_name), free(value), -1);
		existing = ft_getenv(*envp, var_name);
		if (existing)
			new_val = ft_strjoin(existing, value);
		else
			new_val = ft_strdup(value);
		key = ft_strjoin(var_name, "=");
		int ret = replace_add_var(key, new_val, envp);
		return (free(var_name), free(value), free(existing), free(new_val), free(key), ret);
	}

	equal_posit = ft_strchr(av, '=');
	if (equal_posit)
	{
		var_name = ft_substr(av, 0, equal_posit - av + 1);
		value = ft_strdup(equal_posit + 1);
		if (!var_name || !value)
			return (free(var_name), free(value), -1);
		replace_add_var(var_name, value, envp);
		return (free(var_name), free(value), 0);
	}
	if (!ft_getenv(*envp, (char*)av))
		add_var_env(ft_strdup(av), envp_size(*envp), envp);
	return (0);
}
