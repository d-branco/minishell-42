/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards-5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:30:04 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:30:15 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_wildcards(t_tube **res, t_tube *tubes)
{
	char		*tmp_word;

	*res = NULL;
	if (tubes == NULL)
		return ;
	find_wildcard_matches(&tmp_word, tubes->word);
	free(tubes->word);
	tmp_word[ft_strlen(tmp_word)-1] = '\0';
	tubes->word = tmp_word;
	*res = separate_tube(tubes);
	handle_all_wildcards(*res, tubes->next);
	free_tube_lst(tubes);
	return ;
}

void	handle_all_wildcards(t_tube *res, t_tube *t)
{
	t_tube	*last;
	char	*tmp_word;

	if (t == NULL)
		return ;
	find_wildcard_matches(&tmp_word, t->word);
	free(t->word);
	tmp_word[ft_strlen(tmp_word)-1] = '\0';
	t->word = tmp_word;
	last = get_last_tube(res);
	last->next = separate_tube(t);
	handle_all_wildcards(res, t->next);
}

t_tube	*get_last_tube(t_tube *t)
{
	if (t == NULL)
		return (t);
	if (t->next == NULL)
		return (t);
	return (get_last_tube(t->next));
}
