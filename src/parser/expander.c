/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:02:22 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/27 14:01:14 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(const char *name, char **envp)
{
	int		name_len;
	char	*equal_posit;

	name_len = ft_strlen(name);
	while (*envp)
	{
		equal_posit = ft_strchr(*envp, '=');
		if (equal_posit && (equal_posit - *envp == name_len)
			&& ft_strncmp(*envp, name, name_len) == 0)
			return (equal_posit + 1);
		envp++;
	}
	return ("");
}

static char	*get_var_name(const char *str, int *len)
{
	int		i = 0;
	char	*name;

	if (str[i] == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	name = ft_substr(str, 0, i);
	*len = i;
	return (name);
}

static void	append_and_free(char **dst, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst, src);
	free(*dst);
	*dst = tmp;
	free(src);
}

static char	*expand_argument(const char *arg, t_mnsh *shell)
{
	int		i;
	int		var_len;
	char	*res;
	char	*var_name;
	char	*var_value;
	char	quote;

	i = 0;
	res = ft_strdup("");
	//printf("DEBUG 1: arg: %s\n", arg);
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
		{
			quote = arg[i++];
			while (arg[i] && arg[i] != quote)
			{
				if (quote == '"' && arg[i] == '$')
				{
					var_name = get_var_name(&arg[i + 1], &var_len);
					if (ft_strcmp(var_name, "?") == 0)
						var_value = ft_itoa(shell->last_exit_code);
					else
						var_value = ft_strdup(get_env_value(var_name, shell->envp));
					append_and_free(&res, var_value);
					free(var_name);
					i += var_len + 1;
				}
				else
				{
					char tmp[2] = {arg[i++], 0};
					append_and_free(&res, ft_strdup(tmp));
				}
			}
			if (arg[i] == quote)
				i++;
		}
		else if (arg[i] == '$')
		{
			var_name = get_var_name(&arg[i + 1], &var_len);
			if (ft_strcmp(var_name, "?") == 0)
				var_value = ft_itoa(shell->last_exit_code);
			else
				var_value = ft_strdup(get_env_value(var_name, shell->envp));
			append_and_free(&res, var_value);
			free(var_name);
			i += var_len + 1;
		}
		else
		{
			char tmp[2] = {arg[i++], 0};
			append_and_free(&res, ft_strdup(tmp));
		}
	}
//	printf("DEBUG: 2: res: %s\n", res);
	return (res);
}

void	expand_arguments(t_command *cmd, t_mnsh *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->args[i])
	{
		expanded = expand_argument(cmd->args[i], shell);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
//		printf("DEBUG 3 cmd->arg[%d]: %s\n", i, cmd->args[i]);
		i++;
	}
}
