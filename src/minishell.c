/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 16:05:12 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	init_shell(t_mnsh *shell, char **envp);
static char	*init_prompt(int exit_code);
static void	handle_args(int argc, char **argv);
static void	looping_shell(t_env **env);

void		check_args(int argc, char **argv);

void		display_ctrl_c(int toggle);
void		sigint_handler(int sig);
void		silent_signal(int sig);
void		parent_signals(void);
void		silent_signals(void);

int	main(int argc, char **argv, char **envp)
{
	t_mnsh			*shell;
	static t_env	*env[1];

	if (isatty(0) && isatty(2))
		rl_outstream = stderr;
	handle_args(argc, argv);
	shell = ft_malloc(sizeof(t_mnsh) * 1);
	display_ctrl_c(TRUE);
	parent_signals();
	init_shell(shell, envp);
	*env = make_ll_env(shell->envp);
	free_shell(shell);

	check_args(argc, argv);
	looping_shell(env);

	rl_clear_history();
	free_all_env(*env);
	return (handle_exit_code(-1));
}

void	check_args(int argc, char **argv)
{
	int	fd;

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			print_error(0, argv[1], strerror(errno));
			if (errno == ENOENT)
				exit(127);
			else
				exit(126);
		}
		dup2(fd, 0);
		close(fd);
	}
}

static void	looping_shell(t_env **env)
{
	char	*input;
	char	*prompt;

	while (TRUE)
	{
		prompt = init_prompt(handle_exit_code(-1));
		input = readline(prompt);
		free(prompt);
		if (input == NULL)
			break ;
		if (ft_check_input(input))
			add_history(input);
		silent_signals();
		display_ctrl_c(FALSE);
		if (input[0])
			handle_exit_code(parse_n_exec_input(input, env));
		if (*ret_env_key(*env, "EXIT"))
			break ;
		display_ctrl_c(TRUE);
		parent_signals();
	}
	if (isatty(0) && isatty(2))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	display_ctrl_c(FALSE);
}

static void	handle_args(int argc, char **argv)
{
	if (argc > 1)
	{
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		{
			ft_putstr_fd("Minishell: try it with bash!\n", STDERR_FILENO);
			exit (SYNTAX_ERROR);
		}
		else
		{
			ft_putstr_fd("Too many arguments, dear ;)\n", STDERR_FILENO);
			exit (SYNTAX_ERROR);
		}
	}
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
	shell->envp = init_envp(envp);
	handle_shlvl(shell);
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
	while (FALSE)
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
