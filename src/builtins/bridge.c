/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:16:51 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/29 14:02:02 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_env_nodes(t_env *env);
static void	*free_env_array_partial(char **array, int until);
static void	concat_key_value(char *dst, char *key, char *value);
static int	fill_env_array(char **array, t_env *env);

char	**env_to_array(t_env *env)
{
	char	**array;
	int		count;

	count = count_env_nodes(env);
	array = ft_malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	if (fill_env_array(array, env) == -1)
		return (NULL);
	return (array);
}

void	free_env_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
}

static int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	*free_env_array_partial(char **array, int until)
{
	int	i;

	i = 0;
	while (i < until)
		free(array[i++]);
	free(array);
	return (NULL);
}

static void	concat_key_value(char *dst, char *key, char *value)
{
	int	key_len;
	int	value_len;

	key_len = strlen(key);
	value_len = strlen(value);
	ft_memcpy(dst, key, key_len);
	dst[key_len] = '=';
	ft_memcpy(dst + key_len + 1, value, value_len);
	dst[key_len + 1 + value_len] = '\0';
}

static int	fill_env_array(char **array, t_env *env)
{
	t_env	*current;
	int		i;
	int		len;

	current = env;
	i = 0;
	while (current != NULL)
	{
		len = strlen(current->key) + strlen(current->value) + 2;
		array[i] = malloc(sizeof(char) * len);
		if (!array[i])
			return (free_env_array_partial(array, i), -1);
		concat_key_value(array[i], current->key, current->value);
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (0);
}

/*
static int	count_env_nodes(t_env *env);

char	**env_to_array(t_env *env)
{
	int		count;
	char	**array;
	t_env	*current;
	int		i;
	int		key_len;
	int		value_len;
	int		total_len;
	int		j;

	count = count_env_nodes(env);
	array = ft_malloc(sizeof(char *) * (count + 1));
	current = env;
	i = 0;
	while (current != NULL)
	{
		key_len = strlen(current->key);
		value_len = strlen(current->value);
		total_len = key_len + 1 + value_len + 1; // +1 for '=', +1 for '\0'
		array[i] = malloc(sizeof(char) * total_len);
		if (!array[i])
		{
			j = 0;
			while (j < i)
				free(array[j++]);
			free(array);
			return (NULL);
		}
		strcpy(array[i], current->key);//////////////////////// Funcao proibida
		strcat(array[i], "=");///////////////////////////////// Funcao proibida
		strcat(array[i], current->value);////////////////////// Funcao proibida
		current = current->next;
		i++;
	}
	array[count] = NULL;
	return (array);
}

void	free_env_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i] != NULL)
		free(array[i++]);
	free(array);
}

static int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}
	*/
