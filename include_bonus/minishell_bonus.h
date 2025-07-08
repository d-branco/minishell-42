/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:06:16 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:34:55 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include "../libft/libft.h"

# ifndef DEBUG
#  define DEBUG FALSE
# endif

# define TRUE				1
# define FALSE				0

# define SUCCESS			0
# define ERROR				1
# define NON_EXECUTABLE		126
# define UNKNOWN_COMMAND	127

//cd bultin
# define PATH_MAX			1024

# define SYNTAX_ERROR		2

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

typedef enum e_token_type
{
	e_PARENTHESIS_OPEN,
	e_PARENTHESIS_CLOSE,
	e_AND,
	e_OR,
	e_HERE_DOC,
	e_INPUT_REDIRECTION,
	e_APPEND,
	e_OUTPUT_REDIRECTION,
	e_PIPE,
	e_END,
	e_WORD
}	t_tkn_type;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIRECT,
	AST_AND,
	AST_OR
}	t_ast_type;

typedef struct s_mnsh
{
	int					last_exit_code;
	char				*prompt;
	char				**envp;
}	t_mnsh;

typedef struct s_token
{
	t_tkn_type		type;
	char			*str;
	struct s_token	*next;
}	t_token;

typedef struct s_ast
{
	enum e_token_type	type;
	t_list				*pipeline;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	int		in_fd;
	int		out_fd;
	char	*full_path;
	int		status;
	char	*hd_buffer;
	int		builtin;
	char	**args;
	int		i;
	int		n;
}	t_cmd;

typedef struct s_exec
{
	int		n;
	int		**hd_pipes;
	int		**pipes;
	int		fd_count;
	char	**path;
	int		prev;
	t_env	**env;
	t_cmd	*cmds;
}	t_exec;

typedef struct s_tube
{
	char			*word;
	int				modifier;
	int				no_quote_remove;
	struct s_tube	*next;
}	t_tube;

typedef struct s_ast_node
{
	t_ast_type			type;
	void				*content;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

//	char				**args;				Splitted array (NULL-terminated)
typedef struct s_command
{
	char				*command;
	char				**args;
	int					argc;
}	t_command;

//	char				*file;				Filename or heredoc delimiter
typedef struct s_redirect
{
	t_tkn_type			redirect_type;
	char				*file;
}	t_redirect;

typedef struct s_quote_state
{
	int					single_quote;
	int					double_quote;
	int					escaped;
}	t_quote_state;

//src/minishell.c
//int			main(int argc, char **argv, char **envp);
void		check_args(int argc, char **argv);
void		looping_shell(t_env **env);
int			handle_exit_code(int newcode);
//src/utils/erro-0.c
void		print_error(char *program, char *arg, char *msg);
int			calc_len(char *prefix, char *program, char *arg, char *msg);
//src/utils/ft_dprintf.c
int			ft_dprintf(int fd, const char *format, ...);
//src/utils/ft_isspace.c
int			ft_isspace(char chr);
//src/utils/ft_malloc.c
void		*ft_malloc(int total_size);
//src/utils/ft_strcmp.c
int			ft_strcmp(const char *s1, const char *s2);
//src/utils/init_n_etc.c
void		handle_args(int argc, char **argv);
bool		ft_check_input(const char *input);
void		free_shell(t_mnsh *shell);
void		init_shell(t_mnsh *shell, char **envp);
//src/utils/init_utils.c
char		**init_envp(char **envp);
void		handle_shlvl(t_mnsh *shell);
//src//utils/signals.c
void		display_ctrl_c(int toggle);
void		sigint_handler(int sig);
void		silent_signal(int sig);
void		parent_signals(void);
void		silent_signals(void);
//src/execution/parser.c
int			parse_n_exec_input(char *input, t_env **all_env);
t_command	*parse_command(char **input);
int			exec_builtin(int n, char **args, t_env **env, int prev);
char		*lst_to_str(t_list *word);
void		lst_words_len(t_list *word, size_t *len);
//src/execution/parser-0-lexer.c
int			lexer(t_token **tkn, char **str);
int			get_tkn_type(char *str);
const char	*get_tkn_as_str(enum e_token_type n);
t_token		*make_one_tkn(t_token *next, char *str, enum e_token_type type);
int			get_str_token(char **word, char **str);
//src/execution/parser-2.c
int			parse_pipeline(t_list **pipeline, t_token **tok);
int			make_pipeline(t_list **pipeline, t_token **tok);
void		next_tkn(t_token **list);
int			parse_cmd(t_tube **cmd, t_token **tok);
int			parse_tube(t_tube **tube, t_token **tok);
//src/execution/parser-3.c
int			is_redirection(t_token *tkn);
int			get_modifier(t_token *tkn);
t_ast		*make_ast_node(int type, t_ast *lhs, t_ast *rhs, t_list *pipeline);
int			check_tkn(t_token *tok, enum e_token_type expected);
int			tkn_error(t_token *tok);
//src/execution/parser-4.c
void		exit_ve(void);
int			wait_all(int n, int *pids);
int			get_return_value(int status);
int			cmds_with_forks(t_exec *exec);
int			*start_children(t_exec *exec);
//src/execution/parser-5.c
void		command_failure(void);
int			start_child(t_exec *exec, int i);
int			fd_redirect(int input_fd, int output_fd);
void		exec_cmd(t_exec *exec, int i);
void		free_exec(t_exec *exec);
//src/execution/parser-6-fd.c
int			fd_builtin(t_exec *exec, int i);
void		close_fds(t_exec *exec);
int			update_fd_out(t_cmd *cmd, t_tube *redir, t_exec *exec);
int			update_fd_in(t_cmd *cmd, t_tube *redir, t_exec *exec);
//src/execution/parser-7-heredoc.c
void		read_single_heredoc(int *in_fd, char *delim, t_env *env);
void		remove_endl(char *line);
void		heredoc_loop(int write_fd, char *delim, t_env *env);
//src/execution/parser-8.c
int			find_cmd(char **path, char *filename, char **res);
int			find_cmd_cnt(int errno_value);
int			check_file(char *filename, char **res);
int			search_path(char **path, const char *filename, char **res);
char		*concat_slash(const char *str1, const char *str2);
//src/execution/parser-9.c
char		**extract_args(t_tube *lst);
void		init_cmd_path(t_cmd *cmd, t_exec *exec);
int			ret_builtin_enum(char *str);
const char	*ret_builtin_literal(int n);
void		heredoc_failure(void);
//src/execution/parser-10.c
int			**free_pipes(int **pipes, int n);
void		free_cmd(t_cmd *cmd);
void		init_exec(t_list *pipeline, t_exec *exec, t_env **env, int prev);
int			**get_pipes(int n);
char		**extract_path(t_env *env);
//src/execution/parser-11.c
int			pipeline_expansion(t_list **pipeline, t_env *env, int error_code);
int			cmd_expansion(t_tube **cmd, t_env *env, int error_code);
int			expand_tube(t_tube *tube, t_tube **res, t_env *env, int error_code);
void		lst_quote_remove(t_tube *lst);
char		*quote_remove(char *str);
//src/execution/parser-12.c
void		free_ast(t_ast *ast);
void		free_pipeline(t_list *pipeline);
void		free_tube_lst(t_tube *lst);
int			exec_ast(t_ast *ast, t_env **env, int previous);
int			exec_pipeline(t_list *pipeline, t_env **env, int previous);
//src/execution/parser-13.c
int			parse_ast(t_ast **ast, t_token **tkn);
int			parse_tokens(t_ast **ast, t_token **tkn);
int			parse_conditionnal(t_ast **ast, t_token **tkn);
int			handle_quote(char *c, t_quote_state *state);
int			needs_escaping(char c, t_quote_state *state);
//src/execution/parser-14.c
int			make_tkn_lst(t_token **lst, char *str);
void		free_lst_tkn(t_token *lst);
void		tkn_free_one(t_token *tkn);
void		next_token(t_token **list);
//src/execution/parser-15.c
t_exec		*init_pipeline(t_list *pipeline, t_env **env, int prev);
void		init_cmd(t_tube *lst, t_exec *exec, int i);
void		init_redirs(t_tube *lst, t_cmd *cmd, t_exec *exec);
//src/execution/parser-16.c
t_tube		*make_tube(t_tube *new);
t_tube		*separate_tube(t_tube *tube);
char		*get_word(char **str);
//src/expansion/dollar.c
char		*dollar_expansion(
				char **str, t_env *env, int retn, t_quote_state *state);
char		*expand_variable(char **str, t_env *env, t_quote_state *state);
char		*param_expansion(char *str, t_env *env, int retn);
//src/expansion/wildcards.c
int			check_starting_wildcard(
				int *i, char **file, char *expr, char ***sections);
int			check_ending_wildcard(
				int i, char *file, char *expr, char **sections);
void		quote_remove_strarr(char **strarr);
int			check_single_section(int i, int *ret, char **file, char **sections);
//src/expansion/wildcards-2.c
int			match_wildcard(char *file, char *expr);
char		**wildcard_split(char const *s, char c);
char		*wc_next_word(char **str, char const c);
int			wc_count_wrds(char const *s, char const c);
void		ms_split_free(char **s, int i);
//src/expansion/wildcards-3.c
void		check_empty_and_sort(t_list **words, char **ret, char *str);
char		*backslash_chars(char *str, int flag);
void		lst_bubble_sort(
				t_list **lst, int (cmp)(const char *, const char *));
t_list		*lst_swap(t_list *a, t_list *b);
//src/expansion/wildcards-4.c
void		find_wildcard_matches(char **ret, char *str);
int			check_iwatod(char *str, char **ret);
int			contains_wildcards(char *str);
//src/expansion/wildcards-5.c
void		handle_wildcards(t_tube **res, t_tube *tubes);
void		handle_all_wildcards(t_tube *res, t_tube *t);
t_tube		*get_last_tube(t_tube *t);
void		insert_value(char **buf, char *val, int pos, int extra_space);
char		*ret_env_key(t_env *env, char *key);
//src/builtins/bridge.c
char		**env_to_array(t_env *env);
//src/builtins/cd.c
int			exec_cd(char **args, t_env **env, int prev);
//src/builtins/check_builtins.c
int			is_builtin(t_command *cmd);
int			execute_builtin(t_command *cmd, t_mnsh *shell);
void		ft_free_env(char **envp);
char		*ft_getenv(char **envp, char *var_name);
int			envp_size(char **envp);
//src/builtins/echo.c
int			ft_echo(char **av);
int			exec_echo(char **args, t_env **env, int prev);
//src/builtins/env.c
int			ft_env(char **av, char **envp);
int			exec_env(char **args, t_env **env, int prev);
int			exe_env(char **args, t_env **env, int prev);
void		strarr_print(char **s);
//src/builtins/env_utils.c
t_env		*make_env(char *key, char *value, t_env *next);
void		sort_env(t_env **env);
void		ll_swap(char **str1, char **str2);
//src/builtins/env_utils-2.c
t_env		*make_ll_env(char **envp);
void		env_add(t_env **env, char *key, char *value);
t_env		*env_insert(t_env **env, char *key, t_env ***insert_ptr);
void		free_all_env(t_env *env);
void		free_ll_env(t_env *env);
//src/builtins/env_utils-3.c
char		**env_to_strarr(t_env *env);
size_t		size_env(t_env *env);
void		free_strarr(char **s);
//src/builtins/exit.c
int			exec_exit(char **args, t_env **env, int prev);
//int			ft_exit(int ac, char **av, int prev);
//src/builtins/export.c
int			ft_export(char **av, t_mnsh *shell);
//src/builtind/export_utils.c
int			export_var(const char *av, char ***envp);
//src/builtind/export_utils-2.c
int			exec_export(char **args, t_env **env, int prev);
int			export_arg(char *str, t_env **env);
int			split_equal(char *str, char **identifier, char **value);
void		print_export(t_env *env);
//src/builtins/pwd.c
int			ft_pwd(t_mnsh *shell);
char		**init_expo(char **envp);
void		free_env_array(char **array);
int			exec_pwd(char **args, t_env **env, int prev);
//src/builtins/replace_add_var.c
int			replace_add_var(char *var_name, char *value, char ***envp);
int			add_var_env(char *new_var, int size, char ***envp);
//src/builtins/unset.c
int			ft_unset(char **av, char ***envp);
//src/builtins/unset_utils.c
int			exec_unset(char **args, t_env **env, int prev);
void		env_remove(t_env **env, char *key);
int			is_valid_identifier(char *str);
char		*in_quotes(char *str);

#endif
