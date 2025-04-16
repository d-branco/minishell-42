/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:55:05 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/12 16:54:40 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	remove_env_var(char ***envp, const char *var_name);

void	remove_env_var(char ***envp, const char *var_name)
{
	int	count;
	int	i;
	int	j;
	char	**new_env;

	i = 0;
	j = 0;
	count = 0;
	while ((*envp)[count])
		count++;
	new_env = malloc(sizeof(char*) * count);
	while (i < count)
	{
		if (strncmp((*envp)[i], var_name, strlen(var_name)) == 0 && (*envp)[i][strlen(var_name)] == '=')
		{
			//free((*envp)[i]);
			i++;
			continue;
		}
		new_env[j++] = (*envp)[i];
		i++;
	}
	new_env[j] = NULL;
	//free(*envp);
	*envp = new_env;
}

int	ft_unset(char **av, char ***envp)
{
	int	i;
	int	j;

	if (!av[1] || !envp || !*envp)
		return (SUCCESS);
	i = 1;
	while (av[i])
	{
		remove_env_var(envp, av[i]);
		i++;
	}
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	//return (ft_unset(av, &envp));
	
	printf("Antes do unset:\n");
	for (int i = 0; envp[i]; i++)
		printf("%s -- %d\n", envp[i], i);
	int result = ft_unset(av, &envp);
	printf("Depois do unset:\n");
	for (int i = 0; envp[i]; i++)
		printf("%s -- %d\n", envp[i], i);
	//for (int i = 0; envp[i]; i++)
	//	free(envp[i]);
	//free(envp);
	return result;
}
