/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:50:53 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 16:51:40 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	exec_unset(char **args, t_env **env, int prev)
{
	int		ret;
	char	*quoted_id;

	(void)prev;
	ret = 0;
	while (*args)
	{
		if (!is_valid_identifier(*args))
		{
			quoted_id = in_quotes(*args);
			print_error("unset", quoted_id, "not a valid identifier");
			free(quoted_id);
			ret = 1;
		}
		else
		{
			env_remove(env, *args);
		}
		args++;
	}
	return (ret);
}

void	env_remove(t_env **env, char *key)
{
	t_env	*current;
	t_env	*previous;
	t_env	*next;

	current = *env;
	previous = 0;
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			next = current->next;
			if (previous)
				previous->next = next;
			else
				*env = next;
			free_ll_env(current);
			current = NULL;
			break ;
		}
		previous = current;
		current = current->next;
	}
}

int	is_valid_identifier(char *str)
{
	if (ft_isdigit(*str))
		return (0);
	while (ft_isalnum(*str) || *str == '_')
		str++;
	return (*str == '\0');
}

char	*in_quotes(char *str)
{
	int		len;
	char	*res;

	len = ft_strlen(str) + 3;
	res = ft_malloc(1 * len);
	res[0] = '`';
	ft_strlcpy(res + 1, str, len - 1);
	res[len - 2] = '\'';
	res[len - 1] = 0;
	return (res);
}
