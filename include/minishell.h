/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:06:16 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/27 13:24:45 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parser.h"
# include "builtins.h"

# define TRUE			1
# define FALSE			0

# ifndef DEBUG
#  define DEBUG FALSE
# endif

# define SYNTAX_ERROR	2

//		String and Command Line Functions
# include <readline/readline.h>
# include <readline/history.h>
//	char			*readline (const char *prompt);
//	void			rl_clear_history (void);
//	int				rl_on_new_line (void);
//	void			rl_replace_line (const char *text, int clear_undo);
//	void			rl_redisplay (void);
//	void			add_history (const char *string);

//		Standard I/O Functions
# include <stdio.h>
//	int				printf(const char *restrict format, ...);
# include <unistd.h>
//	ssize_t			write(int fd, const void buf[.count], size_t count);
//	ssize_t			read(int fd, void buf[.count], size_t count);
# include <errno.h>
//	void			perror(const char *s);

//		Memory Management
# include <stdlib.h>
//	void			*malloc(size_t size);
//	void			free(void *_Nullable ptr);

//		File Operations
//	int				access(const char *pathname, int mode);
# include <fcntl.h>
//	int				open(const char *pathname, int flags, ..., mode_t mode);
//	int				close(int fd);
//	int				unlink(const char *pathname);
//	char			*getcwd(char buf[.size], size_t size);
//	int				chdir(const char *path);

//		File Status
# include <sys/stat.h>
//	int				stat(const char *restrict pathname,
//						struct stat *restrict statbuf);
//	int				lstat(const char *restrict pathname,
//						struct stat *restrict statbuf);
//	int				fstat(int fd, struct stat *statbuf);

//		Process Control
//	pid_t			fork(void);
# include <sys/wait.h>
//	pid_t			wait(int *_Nullable wstatus);
//	pid_t			waitpid(pid_t pid, int *_Nullable wstatus, int options);
//	pid_t			wait3(int *_Nullable wstatus, int options,
//						struct rusage *_Nullable rusage);
//	pid_t			wait4(pid_t pid, int *_Nullable wstatus, int options,
//						struct rusage *_Nullable rusage);
//	noreturn		exit(int status);
//	int				execve(const char *pathname, char *const _Nullable argv[],
//						char *const _Nullable envp[]);

//		Signal Handling
# include <signal.h>
//typedef typeof(void (int))  *sighandler_t;
//	sighandler_t	signal(int signum, sighandler_t handler);
//	int				sigaction(int signum,
//						const struct sigaction *_Nullable restrict act,
//						struct sigaction *_Nullable restrict oldact);
//	int				sigemptyset(sigset_t *set);
//	int				sigaddset(sigset_t *set, int signum);
//	int				kill(pid_t pid, int sig);

//		File Descriptor Manipulation
//	int				dup(int oldfd);
//	int				dup2(int oldfd, int newfd);
//	int				pipe(int pipefd[2]);

//		Directory Operations
# include <sys/types.h>
# include <dirent.h>
//	DIR				*opendir(const char *name);
# include <dirent.h>
//	struct dirent	*readdir(DIR *dirp);
//	int				closedir(DIR *dirp);

//		Error Handling
# include <string.h>
//	char			*strerror(int errnum);

//		Terminal Functions
//	int				isatty(int fd);
//	char			*ttyname(int fd);
//	int				ttyslot(void);
# include <sys/ioctl.h>
//	int				ioctl(int fd, unsigned long op, ...);
# include <termios.h>
# include <unistd.h>
//	int				tcsetattr(int fd, int optional_actions,
//						const struct termios *termios_p);
//	int				tcgetattr(int fd, struct termios *termios_p);

//		Environment Functions
//	char			*getenv(const char *name);

//		Terminal Capability Functions
# include <curses.h>
# include <term.h>
//extern char PC;
//extern char * UP;
//extern char * BC;
//extern unsigned ospeed;
//	int				tgetent(char *bp, const char *name);
//	int				tgetflag(char *id);
//	int				tgetnum(char *id);
//	char			*tgetstr(char *id, char **area);
//	char			*tgoto(const char *cap, int col, int row);
//	int				tputs(const char *str, int affcnt, int (*putc)(int));

//minishell.c
typedef struct s_mnsh
{
	int					last_exit_code;
	int					export_status;
	char				*prompt;
	char				**envp;
	struct s_ast_node	*ast_head;
}	t_mnsh;

//int			main(int argc, char **argv, char **envp);
void	free_shell(t_mnsh *shell);
int		handle_exit_code(int newcode);
bool	ft_check_input(const char *input);
//execution/exe.c
int		execute_ast(t_ast_node *node, t_mnsh *shell);
//utils/ft_isspace.c
int		ft_isspace(char chr);
//utils/ft_malloc.c
void	*ft_malloc(int total_size);
//utils/ft_strcmp.c
int		ft_strcmp(const char *s1, const char *s2);
//execution/signal.c
void	ft_setup_interactive_signals(void);
void	ft_setup_fork_signals(void);

#endif
