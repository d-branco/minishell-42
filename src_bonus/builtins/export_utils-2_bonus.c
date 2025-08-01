/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils-2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:52:53 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 16:53:14 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	exec_export(char **args, t_env **env, int prev)
{
	int		ret;

	(void)prev;
	ret = 0;
	if (!*args)
		print_export(*env);
	while (*args)
	{
		if (export_arg(*args, env) == -1)
			ret = 1;
		args++;
	}
	return (ret);
}

int	export_arg(char *str, t_env **env)
{
	char	*identifier;
	char	*value;
	char	*quoted;

	split_equal(str, &identifier, &value);
	if (!is_valid_identifier(identifier)
		&& !is_valid_arg((const char *) identifier))
	{
		quoted = in_quotes(str);
		print_error("export", quoted, "not a valid identifier");
		free(quoted);
		return (-1);
	}
	if ((value) && (identifier[(ft_strlen(identifier) - 1)] == '+'))
	{
		identifier[(ft_strlen(identifier) - 1)] = '\0';
		env_append(env, identifier, value);
	}
	else if (value)
		env_add(env, identifier, value);
	else
		env_add(env, identifier, NULL);
	free(identifier);
	free(value);
	return (0);
}

void	env_append(t_env **env, char *key, char *value)
{
	t_env	*found_node;
	t_env	**insert_ptr;
	t_env	*new_node;
	char	*appended_str;

	if (!*key)
		return ;
	found_node = env_insert(env, key, &insert_ptr);
	if (found_node)
	{
		appended_str = ft_strjoin(found_node->value, value);
		free(found_node->value);
		found_node->value = appended_str;
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

int	split_equal(char *str, char **identifier, char **value)
{
	int	i;

	*value = NULL;
	i = 0;
	while (str[i] && (i == 0 || str[i] != '='))
	{
		i++;
	}
	*identifier = ft_substr(str, 0, i);
	if (str[i] == '=')
		*value = ft_strdup(str + i + 1);
	return (0);
}

void	print_export(t_env *env)
{
	t_env	*current;
	char	*escaped;

	sort_env(&env);
	current = env;
	while (current)
	{
		if (current->value)
		{
			escaped = backslash_chars(current->value, TRUE);
			ft_printf("declare -x %s=\"%s\"\n", current->key, escaped);
			free(escaped);
		}
		else
			ft_printf("declare -x %s\n", current->key);
		current = current->next;
	}
}
