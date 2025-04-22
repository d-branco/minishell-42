/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/23 16:20:44 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shell(t_mnsh *shell, char **envp);

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	int		loop;
	t_mnsh	shell;

	init_shell(&shell, envp);
	if (argc > 1)
	{
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
			return (handle_exit_code(parser(argv[2], &shell)),
				free_shell(&shell), handle_exit_code(-1));
		else
			return (ft_putstr_fd("Too many arguments, dear ;)\n", 2), 1);
	}
	if (DEBUG)
		ft_printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG--\n");
	loop = 42; // to be removed
	while (loop)
	{
		input = readline(shell.prompt);
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		else
			handle_exit_code(parser(input, &shell));
		loop--;
	}
	if (DEBUG)
		ft_printf("--DEBUG-- \n--DEBUG-- Goodbye, friend.\n--DEBUG-- \n");
	free_shell(&shell);
	return (handle_exit_code(-1));
}

void	free_shell(t_mnsh *shell)
{
	free(shell->prompt);
	//free ast;
	//free t_mnsh
}

static void	init_shell(t_mnsh *shell, char **envp)
{
	char	*temp;
	int		i;

	temp = ft_itoa(handle_exit_code(-1));
	(void) envp;
	shell->prompt = (char *) ft_malloc(sizeof(char) * 6);
	i = 0;
	while ((i < 3) && temp[i])
	{
		shell->prompt[i] = temp[i];
		i++;
	}
	free(temp);
	shell->prompt[i++] = '%';
	shell->prompt[i++] = ' ';
	while (i < 6)
		shell->prompt[i++] = '\0';
	shell->envp = envp;
	shell->ast_head = NULL;
}

int	handle_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}

//		input = "ls -l | grep .c && echo success || (echo"
//			" failure > output.txt && echo -n \"failure but redirected\")";

//char *input = "ls -l | grep .c && echo success || echo failure > output.txt";
//char *input = "a&&b|c||(d&&e&&(f||g))";
