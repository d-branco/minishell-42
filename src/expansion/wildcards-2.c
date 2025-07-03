/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards-2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:23:49 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:24:22 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	match_wildcard(char *file, char *expr)
{
	char	**section;
	int		i;
	int		ret;

	ret = -1;
	expr = ft_strdup(expr);
	section = wildcard_split(expr, '*');
	quote_remove_strarr(section);
	i = 0;
	if (check_starting_wildcard(&i, &file, expr, &section) == 1)
	{
		free(expr);
		return (0);
	}
	while (check_single_section(i, &ret, &file, section))
		i++;
	if (ret == 1)
		ret = check_ending_wildcard(i, file, expr, section);
	free_strarr(section);
	free(expr);
	return (ret);
}

char	**wildcard_split(char const *s, char c)
{
	char	**ret;
	int		i;
	int		word_count;

	word_count = wc_count_wrds(s, c);
	ret = (char **)ft_malloc(sizeof(char *) * (word_count + 1));
	if (!ret)
		return (NULL);
	i = -1;
	while (++i < word_count)
	{
		ret[i] = wc_next_word((char **)&s, c);
		if (ret[i] == NULL)
		{
			ms_split_free(ret, i);
			return (NULL);
		}
	}
	ret[i] = NULL;
	return (ret);
}

char	*wc_next_word(char **str, char const c)
{
	char			*ret;
	char			*end;
	size_t			len;
	t_quote_state	state;

	while (**str == c && **str != '\0')
		(*str)++;
	end = (char *)*str;
	state = (t_quote_state){0, 0, 0};
	while (*end != '\0' && !(*end == c
			&& !state.escaped && !state.single_quote && !state.double_quote))
	{
		handle_quote(end, &state);
		end++;
	}
	len = end - *str + 1;
	ret = (char *)ft_malloc(sizeof(char) * len);
	if (!ret)
		return (NULL);
	end = ret;
	while ((len--) > 1)
		*end++ = *(*str)++;
	*end = '\0';
	return (ret);
}

int	wc_count_wrds(char const *s, char const c)
{
	int				count;
	int				flag;
	int				old_flag;
	t_quote_state	state;

	flag = -1;
	count = 0;
	state = (t_quote_state){0, 0, 0};
	while (*s)
	{
		old_flag = flag;
		handle_quote((char *)s, &state);
		if (!state.escaped && !state.single_quote && !state.double_quote
			&& *s == c)
			flag = 0;
		else
			flag = 1;
		if (old_flag != flag && flag == 1)
			count++;
		s++;
	}
	return (count);
}

void	ms_split_free(char **s, int i)
{
	while (i >= 0)
	{
		free(s[i]);
		i--;
	}
	free(s);
}
