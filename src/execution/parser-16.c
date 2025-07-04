/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-16.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:17:59 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:18:14 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_tube	*make_tube(t_tube *new)
{
	t_tube	*tube;

	tube = ft_malloc(sizeof(*tube));
	if (new)
	{
		tube->modifier = new->modifier;
		if (new->word)
			tube->word = ft_strdup(new->word);
		else
			tube->word = NULL;
		tube->no_quote_remove = new->no_quote_remove;
		tube->next = NULL;
	}
	else
	{
		tube->modifier = -1;
		tube->word = NULL;
		tube->no_quote_remove = 0;
		tube->next = NULL;
	}
	return (tube);
}

/*
t_tube	*make_tube(t_tube *new)
{
	t_tube	*tube;

	tube = ft_malloc(sizeof(*tube) * 1);
	if (new)
	{
		tube->modifier = new->modifier;
		tube->word = new->word;
		tube->next = new->next;
	}
	else
	{
		tube->modifier = -1;
		tube->word = NULL;
		tube->next = NULL;
	}
	return (tube);
}*/

t_tube	*separate_tube(t_tube *tube)
{
	t_tube	*section;
	t_tube	**current;
	char	*str;
	char	*word;

	section = NULL;
	current = &section;
	str = tube->word;
	while (TRUE)
	{
		word = get_word(&str);
		if (!*word)
		{
			free(word);
			break ;
		}
		*current = make_tube(&(t_tube){word, tube->modifier, tube->no_quote_remove, NULL});
		current = &(*current)->next;
	}
	return (section);
}

char	*get_word(char **str)
{
	char			*word;
	int				i;
	t_quote_state	state;

	state = (t_quote_state){0, 0, 0};
	while (**str && ft_strchr(" 	\n", **str))
		(*str)++;
	i = 0;
	while ((*str)[i])
	{
		handle_quote(*str + i, &state);
		if (!state.escaped && !state.double_quote && !state.single_quote
			&& ft_strchr(" 	\n", (*str)[i]))
			break ;
		i++;
	}
	word = ft_substr((*str), 0, i);
	*str += i;
	return (word);
}
