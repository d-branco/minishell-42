/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:48:14 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:50:50 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_dollar_quotes(char **str);

char	*dollar_expansion(
			char **str, t_env *env, int retn, t_quote_state *state)
{
	char	*ret;

	if ((*str)[1] && ((*str)[1] == '?'))
	{
		ret = ft_itoa(retn);
		*str += 2;
	}
	else if ((*str)[1] && ((*str)[1] == '0'))
	{
		ret = ft_strdup("Shell-A");
		*str += 2;
	}
	else if ((*str)[1] && ((*str)[1] == '*'))
	{
		ret = ft_strdup("");
		*str += 2;
	}
	else
		ret = expand_variable(str, env, state);
	return (ret);
}

char	*expand_variable(char **str, t_env *env, t_quote_state *state)
{
	char	*ret;
	char	*key;
	char	*value;
	int		i;

	i = 1;
	(void) state;
	while (ft_isalnum((*str)[i]) || (*str)[i] == '_')
		i++;
	/*if (i == 1)
	{
		ret = ft_strdup("$");
		*str += i;
		return (ret);
	}*/
	if (i == 1)
	{
		if ((*str)[1] == '\'' || (*str)[1] == '"')
			return (handle_dollar_quotes(str));
		ret = ft_strdup("$");
		*str += i;
		return (ret);
	}
	key = ft_substr(*str, 1, i - 1);
	value = ret_env_key(env, key);
	free(key);
	*str += i;
	if (state->double_quote)
		ret = backslash_chars(value, TRUE);
	else
		ret = backslash_chars(value, FALSE);
	return (ret);
}

char	*ret_env_key(t_env *env, char *key)
{
	static char	empty[1] = "";

	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return (empty);
}

char	*param_expansion(char *str, t_env *env, int retn)
{
	char			*val;
	char			*res;
	int				i;
	t_quote_state	state;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (*str)
	{
		if (*str == '$' && !state.escaped && !state.single_quote)
		{
			if (str[1] == '\'' || str[1] == '"')
				val = handle_dollar_quotes(&str);
			else
				val = dollar_expansion(&str, env, retn, &state);
			insert_value(&res, val, i, ft_strlen(str) + 1);
			i += ft_strlen(val);
			free(val);
		}
		else
		{
			handle_quote(str, &state);
			res[i++] = *(str++);
		}
	}
	res[i] = 0;
	printf("RES: %s\n", res);
	return (res);
}

/*
char	*param_expansion(char *str, t_env *env, int retn)
{
	char			*val;
	char			*res;
	int				i;
	t_quote_state	state;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (*str)
	{
		if (*str == '$' && !state.escaped && !state.single_quote)
		{
			val = dollar_expansion(&str, env, retn, &state);
			insert_value(&res, val, i, (ft_strlen(str) + 1));
			i += ft_strlen(val);
			free(val);
		}
		else
		{
			handle_quote(str, &state);
			res[i++] = *(str++);
		}
	}
	return (res[i] = 0, res);
}
*/
void	insert_value(char **buf, char *val, int pos, int extra_space)
{
	int		len;
	char	*tmp;

	(*buf)[pos] = 0;
	len = ft_strlen(*buf) + ft_strlen(val) + extra_space;
	tmp = ft_malloc(1 * len);
	ft_strlcpy(tmp, *buf, len);
	ft_strlcat(tmp, val, len);
	pos += ft_strlen(val);
	free(*buf);
	*buf = tmp;
}

static char	*handle_dollar_quotes(char **str)
{
	char	quote;
	int		len;
	char	*res;

	quote = (*str)[1]; // ' ou "
	*str += 2; // pula $ e a aspa inicial

	if (**str == quote) // caso especial: $'' ou $""
	{
		*str += 1;
		printf("AQUIIIIII\n");
		return (ft_strdup(""));
	}

	len = 0;
	while ((*str)[len] && (*str)[len] != quote)
		len++;
	res = ft_substr(*str, 0, len);
	*str += len;
	if (**str == quote)
		*str += 1;
	return (res);
}

