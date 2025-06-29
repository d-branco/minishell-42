/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:06:16 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/27 10:53:46 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <stdbool.h>
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
# include <stdarg.h>
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
	struct s_ast_node	*ast_head;
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

typedef struct s_expctx
{
	int					*i;
	char				**res;
	int					*was_quoted;
	t_mnsh				*shell;
}	t_expctx;

//from the previous parser.h
//parser/ast.c
t_ast_node	*build_ast(t_token **tokens);
t_ast_node	*parse_logical_ops(t_token **tokens);
t_ast_node	*parse_pipe(t_token **tokens);
t_ast_node	*parse_redirections(t_token **tokens);
t_ast_node	*create_ast_node(t_ast_type type, void *content);
//parser/ast-print.c
void		print_ast(t_ast_node *node, int depth);
//parser/ast-free.c
void		free_arg_list(t_list *arg_list);
void		free_arg_list_structure(t_list *arg_list);
void		free_ast_node(t_ast_node *node);
//parse/ast-parse-cmd.c
t_ast_node	*parse_commands(t_token **tokens);
//parse/ast-parse-cmd2.c
t_ast_node	*handle_tokens_inside_parenthesis(t_token **tokens);
int			is_valid_token_for_argument(t_token *token);


//other parser. For syntax error
//parser/parser.c
int			parser(char *input, t_mnsh *shell);
int			is_operator(t_token *t);
int			is_redirection(t_token *t);
int			check_token_syntax_errors(t_token *lst);
int			print_unexpected_token(t_token *tkn);
//parser/expander.c
void		expand_arguments(t_command *cmd, t_mnsh *shell);
char		*expand_argument(const char *arg, t_mnsh *shell, int *was_quoted_out);
//parser/expander_two.c
char		*expand_argument(const char *arg, t_mnsh *shell, int *was_quoted_out);
//parser/expander_three.c
char		*get_env_value(const char *name, char **envp);
int			ft_strarr_len(char **arr);
void		append_and_free(char **dst, char *src);
void		append_char(char **res, char c);
//parser/expander_four.c
void		handle_quoted(const char *arg, int *i, char **res, t_mnsh *shell);
void		handle_dollar(const char *arg, int *i, char **res, t_mnsh *shell);

//parser/parser.c
int			parser(char *input, t_mnsh *shell);
//parser/expander.c
void		expand_arguments(t_command *cmd, t_mnsh *shell);
char		*expand_argument(const char *arg, t_mnsh *shell, int *was_quoted_out);
//parser/expander_two.c
char		*expand_argument(const char *arg, t_mnsh *shell, int *was_quoted_out);
//parser/expander_three.c
char		*get_env_value(const char *name, char **envp);
int			ft_strarr_len(char **arr);
void		append_and_free(char **dst, char *src);
void		append_char(char **res, char c);
//parser/expander_four.c
void		handle_quoted(const char *arg, int *i, char **res, t_mnsh *shell);
void		handle_dollar(const char *arg, int *i, char **res, t_mnsh *shell);
//parser/lexer.c
int			parse_input_into_token_list(t_token **list, char *input);
int			validate_syntax(char *str);
int			handle_quoted_string(char *input, int *pos, char **str, char chr);
//parser/lexer-list.c
void		tkn_lst_printer(t_token *lst);
void		tkn_lstclear(t_token **lst);
t_token		*create_token(t_tkn_type token_type, char *token_string);
void		tkn_lstadd_back(t_token **lst, t_token *new);
int			validate_heredoc_syntax(char *input);
//parser/lexer-token.c
void		get_token(t_token **list, char *input, int *pos);
//parser/lexer-tokenizer.c
t_tkn_type	check_type_of_token(char *input, int *pos);
void		isolate_word_token(char *input, int *pos, char **token_string);
void		isolate_operator_token(char *input, int *pos, char **token_string);
//parser/wildcard_bonus.c
char		**expand_argument_and_wildcard(const char *arg, t_mnsh *shell, int *was_quoted_out);
//parser/wildcard_utils_bonus.c
void		ft_strarr_add_back(char ***arr, char *new_str);
void		ft_strarr_extend(char ***dest, char **src);
void		ft_strarr_free(char **arr);
void		ft_strarr_sort(char **arr);

//src/builtins/cd.c
int			ft_cd(int ac, char **av, t_mnsh *shell);
//src/builtins/echo.c
int			ft_echo(char **av);
//src/builtins/env.c
int			ft_env(char **av, char **envp);
//src/builtins/exit.c
int			ft_exit(int ac, char **av, t_mnsh *shell);
//src/builtins/export.c
int			ft_export(char **av, t_mnsh *shell);
//src/builtins/export_utils.c
int			export_var(const char *av, char ***envp);
//src/builtins/pwd.c
int			ft_pwd(t_mnsh *shell);
//src/builtins/unset.c
int			ft_unset(char **av, char ***envp);
int			is_valid_arg(const char *av);
//src/builtins/replace_add_var.c
int			replace_add_var(char *var_name, char *value, char ***envp);
int			add_var_env(char *new_var, int size, char ***envp);
//src/builtins/check_builtins.c
int			is_builtin(t_command *cmd);
int			envp_size(char **envp);
int			execute_builtin(t_command *cmd, t_mnsh *shell);
void		ft_free_env(char **envp);
char		*ft_getenv(char **envp, char *var_name);
//src/builtins/init_utils.c
char		**init_envp(char **envp);
void		handle_shlvl(t_mnsh *shell);

//minishell.c
//int			main(int argc, char **argv, char **envp);
void		free_shell(t_mnsh *shell);
int			handle_exit_code(int newcode);
int			ft_check_input(const char *input);
void		print_error(char *program, char *arg, char *msg);
//execution/exe.c
//int			execute_ast(t_ast_node *node, t_mnsh *shell);
//utils
int			ft_isspace(char chr);
void		*ft_malloc(int total_size);
int			ft_strcmp(const char *s1, const char *s2);
//execution/signal.c
//void		ft_setup_interactive_signals(void);
//void		ft_setup_fork_signals(void);

//parser/ast.c
//t_ast_node	*build_ast(t_token **tokens);
//t_ast_node	*parse_logical_ops(t_token **tokens);
//t_ast_node	*parse_pipe(t_token **tokens);
//t_ast_node	*parse_redirections(t_token **tokens);
//t_ast_node	*create_ast_node(t_ast_type type, void *content);
//parser/ast-print.c
//void		print_ast(t_ast_node *node, int depth);
//parser/ast-free.c
//void		free_arg_list(t_list *arg_list);
//void		free_arg_list_structure(t_list *arg_list);
//void		free_ast_node(t_ast_node *node);
//parse/ast-parse-cmd.c
//t_ast_node	*parse_commands(t_token **tokens);
//parse/ast-parse-cmd2.c
//t_ast_node	*handle_tokens_inside_parenthesis(t_token **tokens);
//int			is_valid_token_for_argument(t_token *token);

//parser/parser.c
int			parse_n_exec_input(char *input, t_mnsh *shell);
t_env		*make_ll_env(char **envp);
void		env_add(t_env **env, char *key, char *value);
t_env		*make_env(char *key, char *value, t_env *next);
void		free_all_env(t_env *env);
void		free_ll_env(t_env *env);

int			make_tkn_lst(t_token **lst, char *str);
//void		tkn_free_lst(t_token *lst);
void		tkn_free_one(t_token *tkn);
void		next_token(t_token **list);

//int			parser(char *input, t_mnsh *shell);
//parser/expander.c
//void		expand_arguments(t_command *cmd, t_mnsh *shell);
//char		*get_env_value(const char *name, char **envp);
////parser/expander_util.c
//void		append_and_free(char **dst, char *src);
//void		append_char(char **res, char c);
////parser/lexer.c
//int			parse_input_into_token_list(t_token **list, char *input);
//int			validate_syntax(char *str);
//void		handle_quoted_string(char *input, int *pos, char **str, char chr);
////parser/lexer-list.c
//void		tkn_lst_printer(t_token *lst);
//void		tkn_lstclear(t_token **lst);
//t_token		*create_token(t_tkn_type token_type, char *token_string);
//void		tkn_lstadd_back(t_token **lst, t_token *new);
//int			validate_heredoc_syntax(char *input);
////parser/lexer-token.c
//void		get_token(t_token **list, char *input, int *pos);
////parser/lexer-tokenizer.c
//t_tkn_type	check_type_of_token(char *input, int *pos);
//void		isolate_word_token(char *input, int *pos, char **token_string);
//void		isolate_operator_token(char *input, int *pos, char **token_string);

//bulti-ins
//src/builtins/cd.c
int			ft_cd(int ac, char **av, t_mnsh *shell);
//src/builtins/echo.c
int			ft_echo(char **av);
//src/builtins/env.c
int			ft_env(char **av, char **envp);
//src/builtins/exit.c
int			ft_exit(int ac, char **av, t_mnsh *shell);
//src/builtins/export.c
int			ft_export(char **av, t_mnsh *shell);
//src/builtins/pwd.c
//int			ft_pwd(void);
//src/builtins/unset.c
int			ft_unset(char **av, char ***envp);
//src/builtins/check_builtins.c
int			is_builtin(t_command *cmd);
int			execute_builtin(t_command *cmd, t_mnsh *shell);
void		ft_free_env(char **envp);
char		*ft_getenv(char **envp, char *var_name);
int			export_var(const char *av, char ***envp);
//src/builtins/init_utils.c
int			replace_add_var(char *var_name, char *value, char ***envp);
char		**init_envp(char **envp);
void		handle_shlvl(t_mnsh *shell);
int			execute_ast(t_ast_node *node, t_mnsh *shell);
//utils/ft_dprintf.c
int			ft_dprintf(int fd, const char *format, ...);
//utils/ft_isspace.c
int			ft_isspace(char chr);
//utils/ft_malloc.c
void		*ft_malloc(int total_size);
//utils/ft_strcmp.c
int			ft_strcmp(const char *s1, const char *s2);
//execution/signal.c
void		ft_setup_interactive_signals(void);
void		ft_setup_fork_signals(void);
void		ft_exec_handler(int signo);

#endif
