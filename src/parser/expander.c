/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:02:22 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/05 14:24:14 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*expand_argument(const char *arg, t_mnsh *shell);
static void	handle_quoted(const char *arg, int *i, char **res, t_mnsh *shell);
static void	handle_dollar(const char *arg, int *i, char **res, t_mnsh *shell);

static char	*get_var_name(const char *str, int *len)
{
	int		i;
	char	*name;

	i = 0;
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

static char	*expand_argument(const char *arg, t_mnsh *shell)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
			handle_quoted(arg, &i, &res, shell);
		else if (arg[i] == '$')
			handle_dollar(arg, &i, &res, shell);
		else
			append_char(&res, arg[i++]);
	}
	return (res);
}

static void	handle_quoted(const char *arg, int *i, char **res, t_mnsh *shell)
{
	char	quote;

	quote = arg[(*i)++];
	while (arg[*i] && arg[*i] != quote)
	{
		if (quote == '"' && arg[*i] == '$')
			handle_dollar(arg, i, res, shell);
		else
			append_char(res, arg[(*i)++]);
	}
	if (arg[*i] == quote)
		(*i)++;
}

static void	handle_dollar(const char *arg, int *i, char **res, t_mnsh *shell)
{
	int		var_len;
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(&arg[*i + 1], &var_len);
	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_itoa(shell->last_exit_code);
	else
		var_value = ft_strdup(get_env_value(var_name, shell->envp));
	append_and_free(res, var_value);
	free(var_name);
	*i += var_len + 1;
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
		i++;
	}
}
