/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils-2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:06:12 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:07:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*make_ll_env(char **envp)
{
	t_env	*ptr;
	char	*key;
	char	*value;
	int		index;

	ptr = NULL;
	while (envp && *envp)
	{
		index = ft_strchr(*envp, '=') - *envp;
		key = ft_substr(*envp, 0, index);
		value = ft_substr(*envp, index + 1, ft_strlen(*envp + index));
		env_add(&ptr, key, value);
		free(key);
		free(value);
		envp++;
	}
	return (ptr);
}

void	env_add(t_env **env, char *key, char *value)
{
	t_env	*found_node;
	t_env	**insert_ptr;
	t_env	*new_node;

	if (!*key)
		return ;
	found_node = env_insert(env, key, &insert_ptr);
	if (found_node)
	{
		free(found_node->value);
		if (value == NULL)
			found_node->value = NULL;
		else
			found_node->value = ft_strdup(value);
	}
	else
	{
		if (value == NULL)
			new_node = make_env(ft_strdup(key), NULL, NULL);
		else
			new_node = make_env(ft_strdup(key), ft_strdup(value), NULL);
		*insert_ptr = new_node;
	}
}

t_env	*env_insert(t_env **env, char *key, t_env ***insert_ptr)
{
	t_env	*current;
	t_env	**ptr;

	ptr = env;
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			*insert_ptr = NULL;
			return (current);
		}
		ptr = &current->next;
		current = current->next;
	}
	*insert_ptr = ptr;
	return (NULL);
}

void	free_all_env(t_env *env)
{
	t_env	*previous;

	previous = 0;
	while (env)
	{
		free_ll_env(previous);
		previous = env;
		env = env->next;
	}
	free_ll_env(previous);
}

void	free_ll_env(t_env *env)
{
	if (env)
	{
		free(env->key);
		free(env->value);
	}
	free(env);
}
