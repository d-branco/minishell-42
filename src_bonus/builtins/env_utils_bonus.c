/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:53:51 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:07:27 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

t_env	*make_env(char *key, char *value, t_env *next)
{
	t_env	*new;

	new = (t_env *)ft_malloc(sizeof(t_env) * 1);
	new->key = key;
	new->value = value;
	new->next = next;
	return (new);
}

void	sort_env(t_env **env)
{
	int		sorted;
	t_env	*current;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		current = *env;
		while (current && current->next)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				sorted = 0;
				ll_swap(&current->key, &current->next->key);
				ll_swap(&current->value, &current->next->value);
			}
			current = current->next;
		}
	}
}

void	ll_swap(char **str1, char **str2)
{
	char	*tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}
