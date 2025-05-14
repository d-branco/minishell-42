/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/14 10:50:41 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	init_shell(t_mnsh *shell, char **envp);
static char	*init_prompt(int exit_code);

int	main(int argc, char **argv, char **envp)
{
	t_mnsh	shell;
	char	*input;
	int		loop;

	init_shell(&shell, envp);
	if (DEBUG)
	{
		loop = -1;
		while (shell.envp[++loop])
			ft_printf("--DEBUG-- [envp] %s\n", shell.envp[loop]);
	}
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
		ft_setup_interactive_signals();
		input = readline(shell.prompt);
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		else
			handle_exit_code(parser(input, &shell));
		free(shell.prompt);
		shell.prompt = init_prompt(handle_exit_code(-1));
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
	//free t_mnsh;
}

static void	init_shell(t_mnsh *shell, char **envp)
{
	(void) envp;

	shell->prompt = init_prompt(handle_exit_code(-1));
	shell->envp = envp;
	shell->ast_head = NULL;
}

static char	*init_prompt(int exit_code)
{
	char	*exit_code_str;
	char	*prompt;
	int		i;

	exit_code_str = ft_itoa(exit_code);
	if (!exit_code_str)
		return (NULL);
	i = ft_strlen(exit_code_str);
	prompt = (char *)ft_malloc(sizeof(char) * (i + 3));
	if (!prompt)
		return (free(exit_code_str), NULL);
	i = 0;
	while (exit_code_str[i])
	{
		prompt[i] = exit_code_str[i];
		i++;
	}
	prompt[i++] = '%';
	prompt[i++] = ' ';
	prompt[i] = '\0';
	free(exit_code_str);
	return (prompt);
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
