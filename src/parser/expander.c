/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:02:22 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/24 23:04:36 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*expand_argument(const char *arg, t_mnsh *shell, int *was_quoted_out)
{
	int		i;
	char	*res;
	int		was_quoted;

	res = ft_strdup("");
	i = 0;
	was_quoted = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
		{
			was_quoted = 1;
			handle_quoted(arg, &i, &res, shell);
		}
		else if (arg[i] == '$')
			handle_dollar(arg, &i, &res, shell);
		else
			append_char(&res, arg[i++]);
	}
	if (!was_quoted && res[0] == '\0')
	{
		free(res);
		res = NULL;
	}
	if (was_quoted_out)
		*was_quoted_out = was_quoted;
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

	if (!arg[*i + 1])
	{
		append_char(res, arg[(*i)++]);
		return ;
	}
	if (arg[*i + 1] == '*')
	{
		append_and_free(res, ft_strdup(""));
		*i += 2;
		return ;
	}
	if (!ft_isalpha(arg[*i + 1]) && arg[*i + 1] != '_' && arg[*i + 1] != '?')
	{
		append_char(res, arg[(*i)++]);
		return ;
	}
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
	char	**new_args;
	char	**expanded;

	new_args = NULL;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded = expand_argument_and_wildcard(cmd->args[i], shell);
		if (expanded)
		{
			int j = 0;
			while (expanded[j])
			{
				bool is_single_token_with_spaces = (expanded[1] == NULL && ft_strchr(expanded[0], ' '));
				bool is_assignment = ft_strchr(expanded[0], '=') != NULL;

				if (is_single_token_with_spaces && !is_assignment)
				{
					char **split = ft_split(expanded[0], ' ');
					if (split)
					{
						int k = 0;
						while (split[k])
							ft_strarr_extend(&new_args, (char *[]){ft_strdup(split[k++]), NULL});
						ft_strarr_free(split);
					}
				}
				else
				{
					ft_strarr_extend(&new_args, (char *[]){ft_strdup(expanded[j]), NULL});
				}
				j++;
			}
			ft_strarr_free(expanded);
		}
		i++;
	}
	ft_strarr_free(cmd->args);
	cmd->args = new_args;
	cmd->argc = ft_strarr_len(cmd->args);
}
