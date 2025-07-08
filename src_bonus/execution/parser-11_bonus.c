/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-11.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:14:23 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:14:35 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	pipeline_expansion(t_list **pipeline, t_env *env, int error_code)
{
	t_list	*current;
	t_tube	**cmd;

	current = *pipeline;
	while (current)
	{
		cmd = (t_tube **)&current->content;
		if (cmd_expansion(cmd, env, error_code) != 0)
			return (-1);
		current = current->next;
	}
	return (0);
}

int	cmd_expansion(t_tube **cmd, t_env *env, int error_code)
{
	t_tube	*current;
	t_tube	*new_section;
	t_tube	*res;
	t_tube	**ptr;
	int		exp;

	exp = 0;
	res = NULL;
	ptr = &res;
	current = *cmd;
	while (current)
	{
		if (expand_tube(current, &new_section, env, error_code) != 0)
			exp = -1;
		*ptr = new_section;
		while (*ptr)
			ptr = &(*ptr)->next;
		current = current->next;
	}
	free_tube_lst(*cmd);
	*cmd = res;
	return (exp);
}

int	expand_tube(t_tube *tube, t_tube **res, t_env *env, int error_code)
{
	char	*tmp;
	char	*word;
	int		code;
	t_tube	*tubo;

	code = 0;
	word = ft_strdup(tube->word);
	if (tube->modifier != e_HERE_DOC)
	{
		tmp = param_expansion(tube->word, env, error_code);
		free(tube->word);
		tube->word = tmp;
		tubo = separate_tube(tube);
		handle_wildcards(res, tubo);
		if (tube->modifier != -1 && (!*res || (*res)->next))
			code = -1;
	}
	else
		*res = make_tube(&(t_tube){word, e_HERE_DOC, 0, NULL});
	lst_quote_remove(*res);
	free(word);
	return (code);
}

void	lst_quote_remove(t_tube *lst)
{
	char	*tmp;

	while (lst)
	{
		if (lst->word && lst->word[0] == '\1')
		{
			tmp = ft_strdup(lst->word + 1);
			free(lst->word);
			lst->word = tmp;
			lst->no_quote_remove = 1;
		}
		if (!lst->no_quote_remove)
		{
			tmp = quote_remove(lst->word);
			free(lst->word);
			lst->word = tmp;
		}
		lst = lst->next;
	}
}

char	*quote_remove(char *str)
{
	int				i;
	char			*res;
	t_quote_state	state;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (*str)
	{
		if (!handle_quote(str, &state))
		{
			res[i] = *str;
			i++;
		}
		str++;
	}
	res[i] = 0;
	return (res);
}
