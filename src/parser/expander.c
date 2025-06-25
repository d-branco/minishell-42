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
	char	*literal;
	int		was_quoted;
	int		start;

	res = ft_strdup("");
	i = 0;
	was_quoted = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1] == '"')
		{
			i += 2;
			start = i;
			while (arg[i] && arg[i] != '"')
				i++;
			literal = ft_substr(arg, start, i - start);
			append_and_free(&res, literal);
			if (arg[i] == '"')
				i++;
			was_quoted = 1;
		}
		else if (arg[i] == '\'' || arg[i] == '"')
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

static int	handle_edge_cases(const char *arg, int *i, char **res)
{
	if (!arg[*i + 1])
	{
		append_char(res, arg[(*i)++]);
		return (1);
	}
	if (arg[*i + 1] == '*')
	{
		append_and_free(res, ft_strdup(""));
		*i += 2;
		return (1);
	}
	if (!ft_isalpha(arg[*i + 1]) && arg[*i + 1] != '_' && arg[*i + 1] != '?')
	{
		append_char(res, arg[(*i)++]);
		return (1);
	}
	return (0);
}

static void	handle_dollar(const char *arg, int *i, char **res, t_mnsh *shell)
{
	int		var_len;
	char	*var_name;
	char	*var_value;

	if (handle_edge_cases(arg, i, res))
		return ;
	var_name = get_var_name(&arg[*i + 1], &var_len);
	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_itoa(shell->last_exit_code);
	else
		var_value = ft_strdup(get_env_value(var_name, shell->envp));
	append_and_free(res, var_value);
	free(var_name);
	*i += var_len + 1;
}

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
				(char *[]){ft_strdup(expanded[j]), NULL});
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
