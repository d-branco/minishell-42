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

#include "../../include_bonus/minishell_bonus.h"

static char	*handle_dollar_quotes(char **str);
static void	handle_normal_char(
				char **str, char *res, int *i, t_quote_state *state);

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
		ret = ft_strdup("C_shell");
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
	if (i == 1)
	{
		if ((*str)[1] == '\'' || (*str)[1] == '"')
			return (handle_dollar_quotes(str));
		*str += i;
		return (ft_strdup("$"));
	}
	key = ft_substr(*str, 1, i - 1);
	value = ret_env_key(env, key);
	free(key);
	*str += i;
	ret = backslash_chars(value, state->double_quote);
	return (ret);
}

static void	handle_normal_char(
		char **str, char *res, int *i, t_quote_state *state)
{
	handle_quote(*str, state);
	res[(*i)++] = *((*str)++);
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
			handle_normal_char(&str, res, &i, &state);
	}
	res[i] = 0;
	return (res);
}

static char	*handle_dollar_quotes(char **str)
{
	char	quote;
	int		len;
	char	*res;
	char	*tagged;

	quote = (*str)[1];
	*str += 2;
	if (**str == quote)
	{
		*str += 1;
		return (ft_strdup("\1"));
	}
	len = 0;
	while ((*str)[len] && (*str)[len] != quote)
		len++;
	res = ft_substr(*str, 0, len);
	*str += len;
	if (**str == quote)
		*str += 1;
	tagged = ft_strjoin("\1", res);
	free(res);
	return (tagged);
}
