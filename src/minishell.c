/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/29 12:49:12 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	init_shell(t_mnsh *shell, char **envp);
static char	*init_prompt(int exit_code);

int	main(int argc, char **argv, char **envp)
{
	t_mnsh	*shell;
	char	*input;
	int		loop;

	shell = ft_malloc(sizeof(t_mnsh) * 1);
	init_shell(shell, envp);
	//if (DEBUG)
	//{
	//	loop = -1;
	//	while (shell->envp[++loop])
	//		ft_printf("--DEBUG-- [envp] %s\n", shell->envp[loop]);
	//}
	if (argc > 1)
	{
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
			return (handle_exit_code(parser(argv[2], shell)),
				free_ast_node(shell->ast_head), free_shell(shell),
				handle_exit_code(-1));
		else
			return (ft_putstr_fd("Too many arguments, dear ;)\n", 2), 1);
	}
	if (DEBUG)
		ft_printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG--\n");
	loop = 42; // to be removed
	while (loop)
	{
		shell->last_exit_code = handle_exit_code(-1);
		input = readline(shell->prompt);
		if (input == NULL)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (ft_check_input(input))
			add_history(input);
		if (input[0])
			handle_exit_code(parser(input, shell));
		free(input);
		free(shell->prompt);
		shell->prompt = init_prompt(handle_exit_code(-1));
		loop--;
	}
	rl_clear_history();
	if (DEBUG)
		ft_printf("--DEBUG-- \n--DEBUG-- Goodbye, friend.\n--DEBUG-- \n");
	free_shell(shell);
	//free (shell);
	return (handle_exit_code(-1));
}

bool	ft_check_input(const char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (true);
		input++;
	}
	return (false);
}

void	free_shell(t_mnsh *shell)
{
	int	i;

	if (shell->prompt)
		free(shell->prompt);
	//free ast;
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
			free(shell->envp[i++]);
		free (shell->envp);
	}
	free (shell);
}

static void	init_shell(t_mnsh *shell, char **envp)
{
	shell->prompt = init_prompt(handle_exit_code(-1));
	shell->envp = init_envp(envp);
	handle_shlvl(shell);
	shell->ast_head = NULL;
	ft_setup_interactive_signals();
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
