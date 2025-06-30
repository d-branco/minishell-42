/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/30 12:58:31 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			parse_n_exec_input(char *input, t_mnsh *shell);

t_env		*make_ll_env(char **envp);
void		env_add(t_env **env, char *key, char *value);
t_env		*make_env(char *key, char *value, t_env *next);
void		free_all_env(t_env *env);
void		free_ll_env(t_env *env);

int			make_tkn_lst(t_token **lst, char *str);
void		free_lst_tkn(t_token *lst);
void		tkn_free_one(t_token *tkn);
void		next_token(t_token **list);

int			lexer(t_token **tkn, char **str);
int			get_tkn_type(char *str);
const char	*get_tkn_as_str(enum e_token_type n);
t_token		*make_one_tkn(t_token *next, char *str, enum e_token_type type);
int			get_str_token(char **word, char **str);

int			handle_quote(char *c, t_quote_state *state);
int			needs_escaping(char c, t_quote_state *state);

int			parse_ast(t_ast **ast, t_token **tkn);
int			parse_tokens(t_ast **ast, t_token **tkn);
int			parse_conditionnal(t_ast **ast, t_token **tkn);

int			exec_ast(t_ast *ast, t_env **env, int previous);
int			exec_pipeline(t_list *pipeline, t_env **env, int previous);

void		free_ast(t_ast *ast);
void		free_pipeline(t_list *pipeline);
void		free_tube_lst(t_tube *lst);

int			pipeline_expansion(t_list **pipeline, t_env *env, int error_code);
int			cmd_expansion(t_tube **cmd, t_env *env, int error_code);
int			expand_tube(t_tube *tube, t_tube **res, t_env *env, int error_code);
void		lst_quote_remove(t_tube *lst);
char		*quote_remove(char *str);

char		*param_expansion(char *str, t_env *env, int retn);
char		*dollar_expansion(
				char **str, t_env *env, int retn, t_quote_state *state);
char		*expand_variable(char **str, t_env *env, t_quote_state *state);
char		*ret_env_key(t_env *env, char *key);
void		insert_value(char **buf, char *val, int *pos, int extra_space);

t_tube		*make_tube(t_tube *new);
t_tube		*separate_tube(t_tube *tube);
char		*get_word(char **str);

t_exec		*init_pipeline(t_list *pipeline, t_env **env, int prev);
void		init_cmd(t_tube *lst, t_exec *exec, int i);
void		init_redirs(t_tube *lst, t_cmd *cmd, t_exec *exec);

char		**env_to_strarr(t_env *env);
size_t		size_env(t_env *env);
void		free_strarr(char **s);

void		init_exec(t_list *pipeline, t_exec *exec, t_env **env, int prev);
int			**get_pipes(int n);
char		**extract_path(t_env *env);

char		**extract_args(t_tube *lst);
void		init_cmd_path(t_cmd *cmd, t_exec *exec);
int			ret_builtin_enum(char *str);
const char	*ret_builtin_literal(int n);

int			find_cmd(char **path, char *filename, char **res);
int			find_cmd_cnt(int errno_value);
int			check_file(char *filename, char **res);
int			search_path(char **path, const char *filename, char **res);
char		*concat_slash(const char *str1, const char *str2);

void		print_error(char *program, char *arg, char *msg);
int			calc_len(char *prefix, char *program, char *arg, char *msg);

int			update_fd_in(t_cmd *cmd, t_tube *redir, t_exec *exec);
void		read_single_heredoc(char **buffer, char *delim);
void		add_line(char **buffer, char *line);
char		*get_input_line(char *prompt);
void		remove_endl(char *line);

int			fd_builtin(t_exec *exec, int i);
void		close_fds(t_exec *exec);
int			update_fd_out(t_cmd *cmd, t_tube *redir, t_exec *exec);

void		free_exec(t_exec *exec);
int			**free_pipes(int **pipes, int n);
void		free_cmd(t_cmd *cmd);

int			cmds_with_forks(t_exec *exec);
int			*start_children(t_exec *exec);
int			start_child(t_exec *exec, int i);
int			fd_redirect(int input_fd, int output_fd);
void		exec_cmd(t_exec *exec, int i);

void		write_all_heredocs(t_exec *exec);
int			wait_all(int n, int *pids);
int			get_return_value(int status);

t_ast		*make_ast_node(int type, t_ast *lhs, t_ast *rhs, t_list *pipeline);
int			check_tkn(t_token *tok, enum e_token_type expected);
int			tkn_error(t_token *tok);

int			parse_pipeline(t_list **pipeline, t_token **tok);
int			make_pipeline(t_list **pipeline, t_token **tok);
void		next_tkn(t_token **list);
int			parse_cmd(t_tube **cmd, t_token **tok);
int			parse_tube(t_tube **tube, t_token **tok);

int			is_redirection(t_token *tkn);
int			get_modifier(t_token *tkn);

void		handle_wildcards(t_tube **res, t_tube *tubes);
void		handle_all_wildcards(t_tube *res, t_tube *t);
t_tube		*get_last_tube(t_tube *t);

void		find_wildcard_matches(char **ret, char *str);
int			check_iwatod(char *str, char **ret);
int			contains_wildcards(char *str);

void		check_empty_and_sort(t_list **words, char **ret, char *str);
char		*backslash_chars(char *str, int flag);
void		lst_bubble_sort(
				t_list **lst, int (cmp)(const char *, const char *));
t_list		*lst_swap(t_list *a, t_list *b);

char		*lst_to_str(t_list *word);
void		lst_words_len(t_list *word, size_t *len);

int			match_wildcard(char *file, char *expr);
char		**wildcard_split(char const *s, char c);
char		*wc_next_word(char **str, char const c);
int			wc_count_wrds(char const *s, char const c);
void		ms_split_free(char **s, int i);

void		quote_remove_strarr(char **strarr);
int			check_single_section(int i, int *ret, char **file, char **sections);

int			check_starting_wildcard(
				int *i, char **file, char *expr, char ***sections);
int			check_ending_wildcard(
				int i, char *file, char *expr, char **sections);

int			exec_builtin(int n, char **args, t_env **env, int prev);
int			exec_pwd(char **args, t_env **env, int prev);
int			exec_cd(char **args, t_env **env, int prev);
int			exec_echo(char **args, t_env **env, int prev);
int			exec_exit(char **args, t_env **env, int prev);

t_command	*parse_command(char **input);

////////////////////////////////////////////////////////////////////////////////
t_command	*parse_command(char **input)
{
	t_command	*cmd;
	int			count;

	cmd = ft_malloc(sizeof(t_command) * 1);
	count = 0;
	if (input && *input)
	{
		while (input[count])
			count++;
	}
	cmd->command = *input;
	cmd->args = input;
	cmd->argc = count;
	return (cmd);
}

////////////////////////////////////////////////////////////////////////////////
//char		**env_to_array(t_env *env);
//void		free_env_array(char **array);
int	exec_pwd(char **args, t_env **env, int prev)
{
	t_mnsh	shell;
	int		ret;

	(void) args;
	(void) prev;
	shell.envp = env_to_array(*env);
	ret = ft_pwd(&shell);
	free_env_array(shell.envp);
	return (ret);
}

int	exec_cd(char **args, t_env **env, int prev)
{
	t_command	*cmd;
	t_mnsh		shell;
	int			ret;

	(void) prev;
	print_error(NULL, NULL, ">>>>TESTE cd: env_to_array");
	shell.envp = env_to_array(*env);
	print_error(NULL, NULL, ">>>>TESTE cd: parse_command");
	cmd = parse_command(args);

	//ft_printf("Command: %s\n", cmd->command);
	//for (int i = 0; i < cmd->argc; i++)///////////////////// TEST
	//	ft_printf("Arg[%d]: %s\n", i, cmd->args[i]);
	//ft_printf("Argc: %d\n", cmd->argc);

	ret = ft_cd(cmd->argc, cmd->args, &shell);

	free_env_array(shell.envp);
	free(cmd);
	return (ret);
}

int	exec_echo(char **args, t_env **env, int prev)
{
	(void) env;
	(void) prev;
	return (ft_echo(args));
}

int	exec_exit(char **args, t_env **env, int prev)
{
	(void) args;
	(void) env;
	(void) prev;
	print_error(NULL, NULL, "WOULD YOU KINDLY MAKE ME");
	return (0);
}

int	exec_builtin(int n, char **args, t_env **env, int prev)
{
	int			ret;
	static int	(*builtin_arr[7])(char **, t_env **, int) = {exec_echo,
		exec_cd, exec_pwd, exec_pwd, exec_pwd, exec_pwd, exec_exit};

	//static int	(*builtin_arr[7])(char **, t_env **, int) = {exec_echo,
	//	exec_cd, exec_pwd, exec_export, exec_unset, exec_env, exec_exit};
	if (n >= 7 || n < 0)
		return (0);
	//shell.envp = env_to_array(*env);
	//cmd = parse_command(args);

	//ft_printf("Command: %s\n", cmd->command);
	//for (int i = 0; i < cmd->argc; i++)///////////////////// TEST
	//	ft_printf("Arg[%d]: %s\n", i, cmd->args[i]);
	//ft_printf("Argc: %d\n", cmd->argc);

	print_error(NULL, NULL, ">>>>TESTE 1");
	//ret = execute_builtin(cmd, &shell);

	//free(cmd);
	//free_env_array(shell.envp);

	ret = ((builtin_arr)[n](args, env, prev));
	print_error(NULL, NULL, ">>>>TESTE 2");
	return (ret);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_n_exec_input(char *input, t_mnsh *shell)
{
	t_token			*lst_tkn;
	t_token			*lst_tkn_origin;
	t_ast			*ast;
	t_env			*ll_env[1];

	if (make_tkn_lst(&lst_tkn, input) != 0)
	{
		handle_exit_code(SYNTAX_ERROR);
		return (handle_exit_code(-1));
	}
	lst_tkn_origin = lst_tkn;
	if (parse_ast(&ast, &lst_tkn) != SYNTAX_ERROR)
	{
		*ll_env = make_ll_env(shell->envp);
		handle_exit_code(exec_ast(ast, ll_env, handle_exit_code(-1)));
		free_all_env(*ll_env);
	}
	else
		handle_exit_code(SYNTAX_ERROR);
	free_ast(ast);
	free_lst_tkn(lst_tkn_origin);
	return (handle_exit_code(-1));
}

////////////////////////////////////////////////////////////////////////////////
void	display_ctrl_c(int toggle)
{
	static struct termios	saved_termios;
	static struct termios	modified_termios;

	if (toggle)
	{
		tcgetattr(STDIN_FILENO, &saved_termios);
		tcgetattr(STDIN_FILENO, &modified_termios);
		modified_termios.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &modified_termios);
	}
	else
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
	}
}

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		handle_exit_code(1);
		write(1, &"\n", 1);
		rl_on_new_line();
		rl_replace_line("\0", 0);
		rl_redisplay();
	}
}

void	silent_signal(int sig)
{
	if (sig == SIGQUIT)
	{
	}
	if (sig == SIGINT)
	{
	}
}

void	parent_signals(void)
{
	struct sigaction	sig_quit;
	struct sigaction	sig_int;

	sig_int.sa_handler = &sigint_handler;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = SIG_IGN;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}

//NOTE SIG_ING because this is inherited by a execve process!
void	silent_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_handler = &silent_signal;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = &silent_signal;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}

////////////////////////////////////////////////////////////////////////////////
int	check_starting_wildcard(int *i, char **file, char *expr, char ***sections)
{
	char	*tmp;

	tmp = quote_remove(expr);
	if (*file[0] == '.' && tmp[0] != '.')
	{
		free_strarr(*sections);
		free(tmp);
		return (1);
	}
	free(tmp);
	if (expr[0] != '*')
	{
		if (0 != ft_strncmp(*file, *sections[0], ft_strlen(*sections[0])))
		{
			free_strarr(*sections);
			return (1);
		}
		*file = *file + ft_strlen(*sections[0]);
		*i = *i + 1;
	}
	return (0);
}

int	check_ending_wildcard(int i, char *file, char *expr, char **sections)
{
	if (expr[ft_strlen(expr) - 1] != '*')
	{
		file -= ft_strlen(sections[i - 1]);
		file += ft_strlen(file) - ft_strlen(sections[i - 1]);
		if (!*sections[i - 1] || ft_strcmp(file, sections[i - 1]) != 0)
			return (0);
	}
	return (TRUE);
}

////////////////////////////////////////////////////////////////////////////////
void	quote_remove_strarr(char **strarr)
{
	int		i;
	char	*tmp;

	i = 0;
	while (strarr[i])
	{
		tmp = quote_remove(strarr[i]);
		free(strarr[i]);
		strarr[i] = tmp;
		i++;
	}
}

int	check_single_section(int i, int *ret, char **file, char **sections)
{
	char	*tmp;

	if (sections[i] == NULL)
	{
		*ret = 1;
		return (0);
	}
	tmp = ft_strnstr(*file, sections[i], ft_strlen(*file));
	if (tmp == NULL)
	{
		*ret = 0;
		return (0);
	}
	*file = tmp + ft_strlen(sections[i]);
	return (1);
}

////////////////////////////////////////////////////////////////////////////////
int	match_wildcard(char *file, char *expr)
{
	char	**section;
	int		i;
	int		ret;

	ret = -1;
	expr = ft_strdup(expr);
	section = wildcard_split(expr, '*');
	quote_remove_strarr(section);
	i = 0;
	if (check_starting_wildcard(&i, &file, expr, &section) == 1)
	{
		free(expr);
		return (0);
	}
	while (check_single_section(i, &ret, &file, section))
		i++;
	if (ret == 1)
		ret = check_ending_wildcard(i, file, expr, section);
	free_strarr(section);
	free(expr);
	return (ret);
}

char	**wildcard_split(char const *s, char c)
{
	char	**ret;
	int		i;
	int		word_count;

	word_count = wc_count_wrds(s, c);
	ret = (char **)ft_malloc(sizeof(char *) * (word_count + 1));
	if (!ret)
		return (NULL);
	i = -1;
	while (++i < word_count)
	{
		ret[i] = wc_next_word((char **)&s, c);
		if (ret[i] == NULL)
		{
			ms_split_free(ret, i);
			return (NULL);
		}
	}
	ret[i] = NULL;
	return (ret);
}

char	*wc_next_word(char **str, char const c)
{
	char			*ret;
	char			*end;
	size_t			len;
	t_quote_state	state;

	while (**str == c && **str != '\0')
		(*str)++;
	end = (char *)*str;
	state = (t_quote_state){0, 0, 0};
	while (*end != '\0' && !(*end == c
			&& !state.escaped && !state.single_quote && !state.double_quote))
	{
		handle_quote(end, &state);
		end++;
	}
	len = end - *str + 1;
	ret = (char *)ft_malloc(sizeof(char) * len);
	if (!ret)
		return (NULL);
	end = ret;
	while ((len--) > 1)
		*end++ = *(*str)++;
	*end = '\0';
	return (ret);
}

int	wc_count_wrds(char const *s, char const c)
{
	int				count;
	int				flag;
	int				old_flag;
	t_quote_state	state;

	flag = -1;
	count = 0;
	state = (t_quote_state){0, 0, 0};
	while (*s)
	{
		old_flag = flag;
		handle_quote((char *)s, &state);
		if (!state.escaped && !state.single_quote && !state.double_quote
			&& *s == c)
			flag = 0;
		else
			flag = 1;
		if (old_flag != flag && flag == 1)
			count++;
		s++;
	}
	return (count);
}

void	ms_split_free(char **s, int i)
{
	while (i >= 0)
	{
		free(s[i]);
		i--;
	}
	free(s);
}

////////////////////////////////////////////////////////////////////////////////
char	*lst_to_str(t_list *word)
{
	char	*ret;
	size_t	size;

	size = 1;
	lst_words_len(word, &size);
	ret = (char *)ft_malloc(sizeof(char) * (size + 1));
	ret[0] = '\0';
	while (word)
	{
		ft_strlcat(ret, word->content, size);
		word = word->next;
	}
	return (ret);
}

void	lst_words_len(t_list *word, size_t *len)
{
	if (!word)
		return ;
	*len += ft_strlen((char *)word->content);
	lst_words_len(word->next, len);
}

////////////////////////////////////////////////////////////////////////////////
void	check_empty_and_sort(t_list **words, char **ret, char *str)
{
	char	*escaped;

	if (*words == NULL)
		*ret = ft_strjoin(str, " ");
	else
	{
		lst_bubble_sort(words, ft_strcmp);
		*ret = lst_to_str(*words);
		escaped = backslash_chars(*ret, 0);
		free(*ret);
		*ret = escaped;
	}
}

char	*backslash_chars(char *str, int flag)
{
	int		i;
	char	*res;
	char	*chars;

	if (flag == 1)
		chars = "\"\\$";
	else
		chars = "\"\\$'";
	res = ft_malloc(sizeof(char) * (2 * ft_strlen(str) + 1));
	i = 0;
	while (*str)
	{
		if (ft_strchr(chars, *str))
		{
			res[i] = '\\';
			i++;
		}
		res[i] = *str;
		i++;
		str++;
	}
	res[i] = 0;
	return (res);
}

void	lst_bubble_sort(t_list **lst, int (cmp)(const char *, const char *))
{
	int		sorted;
	t_list	*current;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		current = *lst;
		while (current && current->next)
		{
			if (0 < cmp(current->content, current->next->content))
			{
				sorted = 0;
				lst_swap(current, current->next);
			}
			current = current->next;
		}
	}
}

t_list	*lst_swap(t_list *a, t_list *b)
{
	void	*tmp;

	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
	return (b);
}

////////////////////////////////////////////////////////////////////////////////
void	find_wildcard_matches(char **ret, char *str)
{
	struct dirent	*dir_entry;
	DIR				*dir;
	t_list			*words;

	if (check_iwatod(str, ret) == -1)
		return ;
	dir = opendir(".");
	if (dir == NULL)
	{
		*ret = ft_strjoin(str, " ");
		return ;
	}
	words = NULL;
	dir_entry = readdir(dir);
	while (dir_entry != NULL)
	{
		if (match_wildcard(dir_entry->d_name, str) == 1)
			ft_lstadd_back(&words,
				ft_lstnew((char *)ft_strjoin(dir_entry->d_name, " ")));
		dir_entry = readdir(dir);
	}
	check_empty_and_sort(&words, ret, str);
	ft_lstclear(&words, free);
	closedir(dir);
}

int	check_iwatod(char *str, char **ret)
{
	if (str == NULL || *str == '\0')
		return (-1);
	if (!contains_wildcards(str))
	{
		*ret = ft_strjoin(str, " ");
		return (-1);
	}
	return (0);
}

int	contains_wildcards(char *str)
{
	int				i;
	t_quote_state	state;

	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (str[i] && (str[i] != '*'
			|| state.escaped || state.single_quote || state.double_quote))
	{
		handle_quote((str + i), &state);
		i++;
	}
	return (str[i] == '*');
}

////////////////////////////////////////////////////////////////////////////////
void	handle_wildcards(t_tube **res, t_tube *tubes)
{
	char		*tmp_word;

	*res = NULL;
	if (tubes == NULL)
		return ;
	find_wildcard_matches(&tmp_word, tubes->word);
	free(tubes->word);
	tmp_word[ft_strlen(tmp_word)-1] = '\0';
	tubes->word = tmp_word;
	*res = separate_tube(tubes);
	handle_all_wildcards(*res, tubes->next);
	free_tube_lst(tubes);
	return ;
}

void	handle_all_wildcards(t_tube *res, t_tube *t)
{
	t_tube	*last;
	char	*tmp_word;

	if (t == NULL)
		return ;
	find_wildcard_matches(&tmp_word, t->word);
	free(t->word);
	tmp_word[ft_strlen(tmp_word)-1] = '\0';
	t->word = tmp_word;
	last = get_last_tube(res);
	last->next = separate_tube(t);
	handle_all_wildcards(res, t->next);
}

t_tube	*get_last_tube(t_tube *t)
{
	if (t == NULL)
		return (t);
	if (t->next == NULL)
		return (t);
	return (get_last_tube(t->next));
}

////////////////////////////////////////////////////////////////////////////////
int	is_redirection(t_token *tkn)
{
	return (tkn->type == e_HERE_DOC
		|| tkn->type == e_INPUT_REDIRECTION
		|| tkn->type == e_APPEND
		|| tkn->type == e_OUTPUT_REDIRECTION);
}

int	get_modifier(t_token *tkn)
{
	if (is_redirection(tkn))
		return (tkn->type);
	else
		return (-1);
}

////////////////////////////////////////////////////////////////////////////////
int	parse_pipeline(t_list **pipeline, t_token **tok)
{
	*pipeline = NULL;
	if ((*tok)->type == e_END)
	{
		tkn_error(*tok);
		return (SYNTAX_ERROR);
	}
	if (make_pipeline(pipeline, tok) == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	return (SUCCESS);
}

int	make_pipeline(t_list **pipeline, t_token **tok)
{
	t_tube	*cmd;
	int		ret;

	ret = parse_cmd(&cmd, tok);
	while (ret == 0)
	{
		ft_lstadd_back(pipeline, ft_lstnew(cmd));
		if ((*tok)->type == e_END || (*tok)->type == e_AND
			|| (*tok)->type == e_OR || (*tok)->type == e_PARENTHESIS_CLOSE)
			break ;
		if (check_tkn(*tok, e_PIPE) != SUCCESS)
		{
			ret = SYNTAX_ERROR;
			break ;
		}
		next_tkn(tok);
		ret = parse_cmd(&cmd, tok);
	}
	if (ret == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	if (!*pipeline)
		return (tkn_error(*tok));
	return (SUCCESS);
}

void	next_tkn(t_token **list)
{
	*list = (*list)->next;
}

int	parse_cmd(t_tube **cmd, t_token **tok)
{
	t_tube	**current;
	int		i;
	int		ret;

	*cmd = NULL;
	current = cmd;
	i = 0;
	ret = parse_tube(current, tok);
	while (ret == 0)
	{
		current = &(*current)->next;
		ret = parse_tube(current, tok);
		i++;
	}
	if (ret == SYNTAX_ERROR)
	{
		return (ret);
	}
	if (i == 0)
	{
		return (tkn_error(*tok));
	}
	return (0);
}

int	parse_tube(t_tube **tube, t_token **tok)
{
	int		modifier;

	*tube = NULL;
	if ((*tok)->type == e_END)
		return (-1);
	if (is_redirection(*tok))
	{
		modifier = get_modifier(*tok);
		next_tkn(tok);
		if (check_tkn(*tok, e_WORD) != 0)
			return (SYNTAX_ERROR);
		*tube = make_tube(&(t_tube){ft_strdup((*tok)->str), modifier, NULL});
		next_tkn(tok);
	}
	else if ((*tok)->type == e_WORD)
	{
		*tube = make_tube(
				&(t_tube){ft_strdup((*tok)->str), -1, NULL});
		next_tkn(tok);
	}
	else
		return (-1);
	return (SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
t_ast	*make_ast_node(int type, t_ast *lhs, t_ast *rhs, t_list *pipeline)
{
	t_ast	*ret;

	ret = ft_malloc(sizeof(*ret) * 1);
	ret->type = type;
	ret->left = lhs;
	ret->right = rhs;
	ret->pipeline = pipeline;
	return (ret);
}

int	check_tkn(t_token *tok, enum e_token_type expected)
{
	if (tok->type != expected)
	{
		return (tkn_error(tok));
	}
	return (SUCCESS);
}

int	tkn_error(t_token *tok)
{
	const char	*tok_str;
	char		*prefix;
	char		*suffix;
	char		*buffer;
	int			len;

	tok_str = get_tkn_as_str(tok->type);
	prefix = "Minishell: syntax error";
	suffix = "'\n";
	len = ft_strlen(tok_str) + ft_strlen(prefix) + ft_strlen(suffix) + 1;
	buffer = ft_malloc(sizeof(*buffer) * len);
	ft_strlcpy(buffer, prefix, len);
	ft_strlcat(buffer, tok_str, len);
	ft_strlcat(buffer, suffix, len);
	write(2, buffer, len - 1);
	free(buffer);
	return (SYNTAX_ERROR);
}

////////////////////////////////////////////////////////////////////////////////
void	write_all_heredocs(t_exec *exec)
{
	int	i;

	i = 0;
	while (i < exec->n)
	{
		if (exec->cmds[i].hd_buffer)
		{
			write(exec->hd_pipes[i][1],
				exec->cmds[i].hd_buffer,
				ft_strlen(exec->cmds[i].hd_buffer));
			close(exec->hd_pipes[i][1]);
		}
		i++;
	}
}

int	wait_all(int n, int *pids)
{
	int	status;
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, WUNTRACED);
		i++;
	}
	return (status);
}

int	get_return_value(int status)
{
	int	exit_code;
	int	sig_num;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else if (WIFSIGNALED(status))
	{
		sig_num = WTERMSIG(status);
		return (128 + sig_num);
	}
	else
	{
		sig_num = WSTOPSIG(status);
		return (128 + sig_num);
	}
}

////////////////////////////////////////////////////////////////////////////////
int	cmds_with_forks(t_exec *exec)
{
	int			*pids;
	int			ret;

	pids = start_children(exec);
	write_all_heredocs(exec);
	close_fds(exec);
	ret = wait_all(exec->n, pids);
	free(pids);
	return (get_return_value(ret));
}

int	*start_children(t_exec *exec)
{
	int	*pids;
	int	i;

	pids = ft_malloc(sizeof(*pids) * (exec->n));
	i = 0;
	while (i < exec->n)
	{
		pids[i] = start_child(exec, i);
		i++;
	}
	return (pids);
}

int	start_child(t_exec *exec, int i)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("forking failed");
		exit(4);
	}
	if (pid == 0)
	{
		fd_redirect(exec->cmds[i].in_fd, exec->cmds[i].out_fd);
		close_fds(exec);
		exec_cmd(exec, i);
		close(0);
		close(1);
		exit(exec->cmds[i].status);
	}
	return (pid);
}

int	fd_redirect(int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	return (0);
}

void	exec_cmd(t_exec *exec, int i)
{
	char	**env_array;

	if (exec->cmds[i].status != 0)
	{
		return ;
	}
	if (exec->cmds[i].builtin != -1)
	{
		exec->cmds[i].status = fd_builtin(exec, i);
	}
	else if (exec->cmds[i].full_path)
	{
		env_array = env_to_strarr(*exec->env);
		execve(exec->cmds[i].full_path, exec->cmds[i].args, env_array);
		print_error(0, exec->cmds[i].full_path, strerror(errno));
		free_strarr(env_array);
		exec->cmds[i].status = 1;
	}
}

////////////////////////////////////////////////////////////////////////////////
void	free_exec(t_exec *exec)
{
	int	i;

	free_pipes(exec->pipes, exec->n - 1);
	free_pipes(exec->hd_pipes, exec->n);
	free_strarr(exec->path);
	i = 0;
	while (i < exec->n)
	{
		free_cmd(&exec->cmds[i]);
		i++;
	}
	free(exec->cmds);
}

int	**free_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	return (0);
}

void	free_cmd(t_cmd *cmd)
{
	free(cmd->full_path);
	free(cmd->hd_buffer);
	free_strarr(cmd->args);
}

////////////////////////////////////////////////////////////////////////////////
int	fd_builtin(t_exec *exec, int i)
{
	int	stdin_clone;
	int	stdout_clone;

	stdin_clone = dup(STDIN_FILENO);
	stdout_clone = dup(STDOUT_FILENO);
	dup2(exec->cmds[i].in_fd, STDIN_FILENO);
	dup2(exec->cmds[i].out_fd, STDOUT_FILENO);
	close_fds(exec);
	exec->cmds[i].status = exec_builtin(exec->cmds[i].builtin,
			exec->cmds[i].args + 1, exec->env, exec->prev);
	dup2(stdin_clone, STDIN_FILENO);
	dup2(stdout_clone, STDOUT_FILENO);
	close(stdin_clone);
	close(stdout_clone);
	return (exec->cmds[i].status);
}

void	close_fds(t_exec *exec)
{
	int	i;

	i = 3;
	while (i < exec->fd_count + 3)
	{
		close(i);
		i++;
	}
}

int	update_fd_out(t_cmd *cmd, t_tube *redir, t_exec *exec)
{
	int	flags;

	if (redir->modifier != e_OUTPUT_REDIRECTION && redir->modifier != e_APPEND)
		return (0);
	flags = O_WRONLY | O_TRUNC | O_CREAT;
	if (redir->modifier == e_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	cmd->out_fd = open(redir->word, flags, 0644);
	if (cmd->out_fd < 0)
	{
		print_error(0, redir->word, strerror(errno));
		cmd->status = 1;
		return (-1);
	}
	exec->fd_count++;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
int	update_fd_in(t_cmd *cmd, t_tube *redir, t_exec *exec)
{
	if (redir->modifier == e_INPUT_REDIRECTION)
	{
		cmd->in_fd = open(redir->word, O_RDONLY);
		if (cmd->in_fd < 0)
		{
			print_error(0, redir->word, strerror(errno));
			cmd->status = 1;
			return (-1);
		}
		exec->fd_count++;
	}
	else if (redir->modifier == e_HERE_DOC)
	{
		read_single_heredoc(&cmd->hd_buffer, redir->word);
		cmd->in_fd = exec->hd_pipes[cmd->i][0];
	}
	return (0);
}

void	read_single_heredoc(char **buffer, char *delim)
{
	char	*line;

	free(*buffer);
	*buffer = ft_strdup("");
	while (TRUE)
	{
		line = get_input_line("> ");
		if (line == NULL || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		add_line(buffer, line);
	}
}

void	add_line(char **buffer, char *line)
{
	char	*tmp;
	int		size;

	size = ft_strlen(line) + 2;
	if (*buffer)
		size += ft_strlen(*buffer);
	tmp = ft_malloc(sizeof(*tmp) * size);
	if (*buffer)
	{
		ft_strlcpy(tmp, *buffer, size);
		ft_strlcat(tmp, line, size);
	}
	else
		ft_strlcpy(tmp, line, size);
	tmp[size - 2] = '\n';
	tmp[size - 1] = '\0';
	free(*buffer);
	free(line);
	*buffer = tmp;
}

char	*get_input_line(char *prompt)
{
	char	*line;

	if (isatty(0) && isatty(2))
		line = readline(prompt);
	else
	{
		line = get_next_line(0);
		remove_endl(line);
	}
	return (line);
}

void	remove_endl(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
		{
			line[i] = '\0';
			break ;
		}
		i++;
	}
}

////////////////////////////////////////////////////////////////////////////////
void	print_error(char *program, char *arg, char *msg)
{
	int		len;
	char	*prefix;
	char	*buffer;

	prefix = "Minishell";
	len = calc_len(prefix, program, arg, msg);
	buffer = ft_malloc(1 * len);
	ft_strlcpy(buffer, prefix, len);
	ft_strlcat(buffer, ": ", len);
	if (program)
	{
		ft_strlcat(buffer, program, len);
		ft_strlcat(buffer, ": ", len);
	}
	if (arg)
	{
		ft_strlcat(buffer, arg, len);
		ft_strlcat(buffer, ": ", len);
	}
	ft_strlcat(buffer, msg, len);
	ft_strlcat(buffer, "\n", len);
	write(2, buffer, len - 1);
	free(buffer);
}

int	calc_len(char *prefix, char *program, char *arg, char *msg)
{
	int	len;

	len = ft_strlen(prefix) + 2 + ft_strlen(msg) + 2;
	if (program)
		len += ft_strlen(program) + 2;
	if (arg)
		len += ft_strlen(arg) + 2;
	return (len);
}

////////////////////////////////////////////////////////////////////////////////
int	find_cmd(char **path, char *filename, char **res)
{
	int			errno_value;

	errno_value = ENOENT;
	*res = 0;
	if (ft_strchr(filename, '/') != NULL)
	{
		errno_value = check_file(filename, res);
		if (errno_value != 0)
			print_error(0, filename, strerror(errno_value));
	}
	else
	{
		if (filename[0] == '\0')
			errno_value = ENOENT;
		else
			errno_value = search_path(path, filename, res);
		if (errno_value != 0)
		{
			if (errno_value == ENOENT)
				print_error(0, filename, "command not found");
			else
				print_error(0, filename, strerror(errno_value));
		}
	}
	return (find_cmd_cnt(errno_value));
}

int	find_cmd_cnt(int errno_value)
{
	if (errno_value == 0)
		return (0);
	if (errno_value == ENOENT)
		return (127);
	return (126);
}

int	check_file(char *filename, char **res)
{
	struct stat	path_info;

	*res = 0;
	if (access(filename, F_OK) == -1)
		return (errno);
	if (lstat(filename, &path_info) != 0)
	{
		print_error(0, 0, strerror(errno));
		exit(5);
	}
	if (S_ISDIR(path_info.st_mode))
		return (EISDIR);
	if (access(filename, X_OK) == -1)
		return (errno);
	*res = ft_strdup(filename);
	return (0);
}

int	search_path(char **path, const char *filename, char **res)
{
	int		i;
	int		status;
	int		ret;
	char	*concat;

	ret = ENOENT;
	if (path)
	{
		i = 0;
		while (path[i])
		{
			concat = concat_slash(path[i], filename);
			status = check_file(concat, res);
			free(concat);
			if (status == 0)
				return (0);
			else if (status != ENOENT)
				ret = status;
			i++;
		}
	}
	*res = 0;
	return (ret);
}

char	*concat_slash(const char *str1, const char *str2)
{
	char	*res;
	size_t	buffer_size;

	buffer_size = ft_strlen(str1) + ft_strlen(str2) + 2;
	res = ft_malloc(buffer_size * 1);
	ft_strlcpy(res, str1, buffer_size);
	ft_strlcat(res, "/", buffer_size);
	ft_strlcat(res, str2, buffer_size);
	return (res);
}

////////////////////////////////////////////////////////////////////////////////
char	**extract_args(t_tube *lst)
{
	t_tube	*current;
	char	**args;
	int		i;

	i = 0;
	current = lst;
	while (current)
	{
		if (current->modifier == -1)
			i++;
		current = current->next;
	}
	args = ft_malloc(sizeof(*args) * (i + 1));
	current = lst;
	i = 0;
	while (current)
	{
		if (current->modifier == -1)
			args[i++] = ft_strdup(current->word);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

void	init_cmd_path(t_cmd *cmd, t_exec *exec)
{
	if (!cmd->args[0])
		return ;
	if (ret_builtin_enum(cmd->args[0]) != -1)
		cmd->builtin = ret_builtin_enum(cmd->args[0]);
	else
		cmd->status = find_cmd(exec->path, cmd->args[0], &(cmd->full_path));
}

int	ret_builtin_enum(char *str)
{
	int		i;

	i = -1;
	while (++i < 7)
	{
		if (!ft_strcmp(str, ret_builtin_literal(i)))
			return (i);
	}
	return (-1);
}

const char	*ret_builtin_literal(int n)
{
	static const char	*tkn_str[7] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"};

	if (n > 7 || n < 0)
		return (NULL);
	return (tkn_str[n]);
}

////////////////////////////////////////////////////////////////////////////////
void	init_exec(t_list *pipeline, t_exec *exec, t_env **env, int prev)
{
	int		n_pipes;

	exec->n = ft_lstsize(pipeline);
	if (exec->n != 0)
		n_pipes = exec->n - 1;
	else
		n_pipes = 0;
	exec->pipes = get_pipes(n_pipes);
	exec->hd_pipes = get_pipes(exec->n);
	exec->fd_count = n_pipes * 2 + exec->n * 2;
	exec->env = env;
	exec->prev = prev;
	exec->path = extract_path(*env);
	exec->cmds = ft_malloc(sizeof(*(exec->cmds)) * (exec->n));
}

int	**get_pipes(int n)
{
	int	**pipes;
	int	i;

	pipes = ft_malloc(sizeof(*pipes) * n);
	i = 0;
	while (i < n)
	{
		pipes[i] = ft_malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) != 0)
		{
			perror("failed to open pipe");//////////////////////////////////////
			exit(3);
		}
		i++;
	}
	return (pipes);
}

char	**extract_path(t_env *env)
{
	int		i;
	char	**env_array;
	char	**ret;

	ret = NULL;
	env_array = env_to_strarr(env);
	if (env_array)
	{
		i = 0;
		while (env_array[i])
		{
			if (ft_strncmp("PATH=", env_array[i], 5) == 0)
			{
				ret = ft_split(env_array[i] + 5, ':');
				break ;
			}
			i++;
		}
	}
	free_strarr(env_array);
	if (ret)
		return (ret);
	return (ft_split("", ':'));
}

////////////////////////////////////////////////////////////////////////////////
char	**env_to_strarr(t_env *env)
{
	char	**ret;
	char	*tmp;
	int		size;
	int		i;

	size = size_env(env);
	ret = ft_malloc(sizeof(*ret) * (size + 1));
	i = 0;
	while (i < size)
	{
		tmp = ft_strjoin(env->key, "=");
		ret[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		env = env->next;
		i++;
	}
	ret[i] = 0;
	return (ret);
}

size_t	size_env(t_env *env)
{
	size_t	len;

	len = 0;
	while (env != NULL)
	{
		env = env->next;
		len++;
	}
	return (len);
}

void	free_strarr(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		free(s[i]);
		i--;
	}
	free(s);
}

////////////////////////////////////////////////////////////////////////////////
t_exec	*init_pipeline(t_list *pipeline, t_env **env, int prev)
{
	t_exec	*exec;
	int		i;

	exec = ft_malloc(sizeof(*exec) * 1);
	init_exec(pipeline, exec, env, prev);
	i = 0;
	while (pipeline)
	{
		init_cmd(pipeline->content, exec, i);
		pipeline = pipeline->next;
		i++;
	}
	return (exec);
}

void	init_cmd(t_tube *lst, t_exec *exec, int i)
{
	t_cmd	*cmd;

	cmd = &exec->cmds[i];
	cmd->status = 0;
	cmd->full_path = NULL;
	cmd->hd_buffer = NULL;
	cmd->builtin = -1;
	cmd->args = NULL;
	cmd->i = i;
	cmd->n = exec->n;
	cmd->args = extract_args(lst);
	init_redirs(lst, cmd, exec);
	if (cmd->status == 0)
		init_cmd_path(cmd, exec);
}

void	init_redirs(t_tube *lst, t_cmd *cmd, t_exec *exec)
{
	t_tube	*current;

	if (cmd->i == 0)
		cmd->in_fd = STDIN_FILENO;
	else
		cmd->in_fd = exec->pipes[cmd->i - 1][0];
	if (cmd->i == cmd->n - 1)
		cmd->out_fd = STDOUT_FILENO;
	else
		cmd->out_fd = exec->pipes[cmd->i][1];
	current = lst;
	while (current)
	{
		if (update_fd_in(cmd, current, exec) == -1)
			break ;
		if (update_fd_out(cmd, current, exec) == -1)
			break ;
		current = current->next;
	}
}

////////////////////////////////////////////////////////////////////////////////
t_tube	*make_tube(t_tube *new)
{
	t_tube	*tube;

	tube = ft_malloc(sizeof(*tube) * 1);
	if (new)
	{
		tube->modifier = new->modifier;
		tube->word = new->word;
		tube->next = new->next;
	}
	else
	{
		tube->modifier = -1;
		tube->word = NULL;
		tube->next = NULL;
	}
	return (tube);
}

t_tube	*separate_tube(t_tube *tube)
{
	t_tube	*section;
	t_tube	**current;
	char	*str;
	char	*word;

	section = NULL;
	current = &section;
	str = tube->word;
	while (TRUE)
	{
		word = get_word(&str);
		if (!*word)
		{
			free(word);
			break ;
		}
		*current = make_tube(&(t_tube){word, tube->modifier, NULL});
		current = &(*current)->next;
	}
	return (section);
}

char	*get_word(char **str)
{
	char			*word;
	int				i;
	t_quote_state	state;

	state = (t_quote_state){0, 0, 0};
	while (**str && ft_strchr(" 	\n", **str))
		(*str)++;
	i = 0;
	while ((*str)[i])
	{
		handle_quote(*str + i, &state);
		if (!state.escaped && !state.double_quote && !state.single_quote
			&& ft_strchr(" 	\n", (*str)[i]))
			break ;
		i++;
	}
	word = ft_substr((*str), 0, i);
	*str += i;
	return (word);
}

////////////////////////////////////////////////////////////////////////////////
char	*param_expansion(char *str, t_env *env, int retn)
{
	char			*val;
	char			*res;
	int				i;
	t_quote_state	state;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (*str)
	{
		if (*str == '$' && !state.escaped && !state.single_quote)
		{
			val = dollar_expansion(&str, env, retn, &state);
			insert_value(&res, val, &i, (ft_strlen(str) + 1));
			free(val);
		}
		else
		{
			handle_quote(str, &state);
			res[i++] = *(str++);
		}
	}
	return (res[i] = 0, res);
}

char	*dollar_expansion(
			char **str, t_env *env, int retn, t_quote_state *state)
{
	char	*ret;

	if ((*str)[1] && ((*str)[1] == '?'))
		ret = ft_itoa(retn);
	else if ((*str)[1] && ((*str)[1] == '?'))
		ret = ft_strdup("Minishell: Shell-A");
	else
		ret = expand_variable(str, env, state);
	return (ret);
}

char	*expand_variable(char **str, t_env *env, t_quote_state *state)
{
	char	*ret;
	char	*key;
	char	*value;
	int		i;

	i = 1;
	(void) state;
	while (ft_isalnum((*str)[i]) || (*str)[i] == '_')
		i++;
	if (i == 1)
	{
		ret = ft_strdup("$");
		*str += i;
		return (ret);
	}
	key = ft_substr(*str, 1, i - 1);
	value = ret_env_key(env, key);
	free(key);
	*str += i;
	if (state)
		ret = backslash_chars(value, TRUE);
	else
		ret = backslash_chars(value, FALSE);
	return (ret);
}

char	*ret_env_key(t_env *env, char *key)
{
	static char	empty[1] = "";

	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return (empty);
}

void	insert_value(char **buf, char *val, int *pos, int extra_space)
{
	int		len;
	char	*tmp;

	(*buf)[*pos] = 0;
	len = ft_strlen(*buf) + ft_strlen(val) + extra_space;
	tmp = ft_malloc(1 * len);
	ft_strlcpy(tmp, *buf, len);
	ft_strlcat(tmp, val, len);
	*pos += ft_strlen(val);
	free(*buf);
	*buf = tmp;
}

////////////////////////////////////////////////////////////////////////////////
int	pipeline_expansion(t_list **pipeline, t_env *env, int error_code)
{
	t_list	*current;
	t_tube	**cmd;

	current = *pipeline;
	while (current)
	{
		cmd = (t_tube **)&current->content;
		if (cmd_expansion(cmd, env, error_code) != 0)
			return (-1);
		current = current->next;
	}
	return (0);
}

int	cmd_expansion(t_tube **cmd, t_env *env, int error_code)
{
	t_tube	*current;
	t_tube	*new_section;
	t_tube	*res;
	t_tube	**ptr;
	int		exp;

	exp = 0;
	res = NULL;
	ptr = &res;
	current = *cmd;
	while (current)
	{
		if (expand_tube(current, &new_section, env, error_code) != 0)
			exp = -1;
		*ptr = new_section;
		while (*ptr)
			ptr = &(*ptr)->next;
		current = current->next;
	}
	free_tube_lst(*cmd);
	*cmd = res;
	return (exp);
}

int	expand_tube(t_tube *tube, t_tube **res, t_env *env, int error_code)
{
	char	*tmp;
	char	*word;
	int		code;
	t_tube	*tubo;

	code = 0;
	word = ft_strdup(tube->word);
	if (tube->modifier != e_HERE_DOC)
	{
		tmp = param_expansion(tube->word, env, error_code);
		free(tube->word);
		tube->word = tmp;
		tubo = separate_tube(tube);
		handle_wildcards(res, tubo);
		if (tube->modifier != -1 && (!*res || (*res)->next))
			code = -1;
	}
	else
		*res = make_tube(&(t_tube){ft_strdup(word), e_HERE_DOC, NULL});
	lst_quote_remove(*res);
	free(word);
	return (code);
}

void	lst_quote_remove(t_tube *lst)
{
	char	*tmp;

	while (lst)
	{
		tmp = quote_remove(lst->word);
		free(lst->word);
		lst->word = tmp;
		lst = lst->next;
	}
}

char	*quote_remove(char *str)
{
	int				i;
	char			*res;
	t_quote_state	state;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	state = (t_quote_state){0, 0, 0};
	i = 0;
	while (*str)
	{
		if (!handle_quote(str, &state))
		{
			res[i] = *str;
			i++;
		}
		str++;
	}
	res[i] = 0;
	return (res);
}

////////////////////////////////////////////////////////////////////////////////
void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (!ast->right && !ast->left)
	{
		free_pipeline(ast->pipeline);
		free(ast);
		return ;
	}
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast);
}

void	free_pipeline(t_list *pipeline)
{
	ft_lstclear(&pipeline, (void (*)(void *))free_tube_lst);
}

void	free_tube_lst(t_tube *lst)
{
	t_tube	*previous;

	previous = 0;
	while (lst)
	{
		if (previous)
			free(previous->word);
		free(previous);
		previous = lst;
		lst = lst->next;
	}
	if (previous)
		free(previous->word);
	free(previous);
}

////////////////////////////////////////////////////////////////////////////////
int	exec_ast(t_ast *ast, t_env **env, int previous)
{
	int	error_code;

	if (!ast)
		return (0);
	if (!ast->left && !ast->right)
	{
		if (pipeline_expansion(&ast->pipeline, *env, previous) != 0)
			return (1);
		return (exec_pipeline(ast->pipeline, env, previous));
	}
	error_code = exec_ast(ast->left, env, previous);
	if (!(!error_code == (ast->type == e_OR)))
		error_code = exec_ast(ast->right, env, previous);
	return (error_code);
}

int	exec_pipeline(t_list *pipeline, t_env **env, int previous)
{
	t_exec	*exec;
	int		error_code;

	exec = init_pipeline(pipeline, env, previous);
	error_code = 0;
	if (exec->n == 1 && exec->cmds[0].builtin != -1)
		error_code = fd_builtin(exec, 0);
	else if (exec->n >= 1)
		error_code = cmds_with_forks(exec);
	free_exec(exec);
	free(exec);
	return (error_code);
}

////////////////////////////////////////////////////////////////////////////////
int	parse_ast(t_ast **ast, t_token **tkn)
{
	*ast = NULL;
	if ((*tkn)->type == e_END)
		return (SUCCESS);
	if (parse_tokens(ast, tkn) != SUCCESS)
		return (SYNTAX_ERROR);
	if ((*tkn)->type != e_END)
	{
		ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int	parse_tokens(t_ast **ast, t_token **tkn)
{
	t_ast	*ptr;
	int		tkn_type;

	*ast = NULL;
	if (parse_conditionnal(ast, tkn) != SUCCESS)
		return (SYNTAX_ERROR);
	while (TRUE)
	{
		if ((*tkn)->type == e_END || (*tkn)->type == e_PARENTHESIS_CLOSE)
			break ;
		if ((*tkn)->type != e_AND && (*tkn)->type != e_OR)
		{
			ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
			return (SYNTAX_ERROR);
		}
		tkn_type = (*tkn)->type;
		next_token(tkn);
		if (parse_conditionnal(&ptr, tkn) != SUCCESS)
		{
			free_ast(ptr);
			return (SYNTAX_ERROR);
		}
		*ast = make_ast_node(tkn_type, *ast, ptr, NULL);
	}
	return (SUCCESS);
}

int	parse_conditionnal(t_ast **ast, t_token **tkn)
{
	t_list	*pipeline;

	*ast = NULL;
	if ((*tkn)->type == e_PARENTHESIS_OPEN)
	{
		next_token(tkn);
		if (parse_tokens(ast, tkn) != SUCCESS)
			return (SYNTAX_ERROR);
		if (check_tkn(*tkn, e_PARENTHESIS_CLOSE) != SUCCESS)
			return (SYNTAX_ERROR);
		next_token(tkn);
	}
	else
	{
		if (parse_pipeline(&pipeline, tkn) != SUCCESS)
		{
			free_pipeline(pipeline);
			return (SYNTAX_ERROR);
		}
		*ast = make_ast_node(e_PIPE, NULL, NULL, pipeline);
	}
	return (SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
int	lexer(t_token **tkn, char **str)
{
	int		error_code;
	char	*word;
	int		tkn_type;

	error_code = 0;
	while (ft_isspace(**str))
		(*str)++;
	tkn_type = get_tkn_type(*str);
	if (tkn_type != e_WORD)
	{
		*tkn = make_one_tkn(NULL, NULL, tkn_type);
		if (tkn_type != e_END)
			*str += ft_strlen(get_tkn_as_str(tkn_type));
	}
	else
	{
		error_code = get_str_token(&word, str);
		*tkn = make_one_tkn(NULL, word, e_WORD);
	}
	return (error_code);
}

int	get_tkn_type(char *str)
{
	int			i;
	const char	*tkn_str;

	if (*str == '\0' || *str == '\n')
		return (e_END);
	i = 0;
	while (i < 9)
	{
		tkn_str = get_tkn_as_str(i);
		if (!ft_strncmp(str, tkn_str, ft_strlen(tkn_str)))
			return (i);
		i++;
	}
	return (e_WORD);
}

const char	*get_tkn_as_str(enum e_token_type n)
{
	static const char	*tkn_strings[9] = {
		"(", ")", "&&", "||", "<<", "<", ">>", ">", "|"};

	if (n == e_END)
		return ("EOL");
	if (n > 9 || n < 0)
		return (NULL);
	return (tkn_strings[n]);
}

t_token	*make_one_tkn(t_token *next, char *str, enum e_token_type type)
{
	t_token	*new;

	new = (t_token *)ft_malloc(sizeof(t_token) * 1);
	new->next = next;
	new->str = str;
	new->type = type;
	return (new);
}

int	get_str_token(char **word, char **str)
{
	int				i;
	t_quote_state	state;

	state = (t_quote_state){0, 0, 0};
	i = 0;
	while ((*str)[i] && (!ft_isspace((*str)[i])
		|| state.escaped || state.single_quote || state.double_quote))
	{
		handle_quote(*str + i, &state);
		if (!state.escaped && !state.single_quote && !state.double_quote
			&& get_tkn_type(*str + i) != e_WORD)
			break ;
		i++;
	}
	*word = ft_substr(*str, 0, i);
	*str += i;
	if (state.single_quote || state.double_quote)
	{
		ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
		return (handle_exit_code(2));
	}
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	handle_quote(char *c, t_quote_state *state)
{
	int	ret;

	ret = 1;
	if (*c == '\'' && !state->double_quote && !state->escaped)
	{
		state->single_quote = !state->single_quote;
	}
	else if (*c == '"' && !state->single_quote && !state->escaped)
	{
		state->double_quote = !state->double_quote;
	}
	else if (*c == '\\' && !state->single_quote && !state->escaped
		&& needs_escaping(*(c + 1), state))
	{
		state->escaped = TRUE;
	}
	else
	{
		ret = 0;
		state->escaped = FALSE;
	}
	return (ret);
}

int	needs_escaping(char c, t_quote_state *state)
{
	return (c == '\\' || c == '"' || (c == '\'' && !state->double_quote)
		|| (c == '$') || (c == '*' && !state->double_quote));
}

////////////////////////////////////////////////////////////////////////////////
int	make_tkn_lst(t_token **lst, char *str)
{
	t_token	*end;
	t_token	*tmp;

	*lst = NULL;
	while (1)
	{
		if (lexer(&tmp, &str) != 0)
		{
			tkn_free_one(tmp);
			free_lst_tkn(*lst);
			return (-1);
		}
		if (*lst == NULL)
		{
			*lst = tmp;
			end = *lst;
		}
		else
		{
			end->next = tmp;
			end = end->next;
		}
		if (tmp->type == e_END)
			return (0);
	}
}

void	free_lst_tkn(t_token *lst)
{
	t_token	*previous;

	previous = 0;
	while (lst)
	{
		tkn_free_one(previous);
		previous = lst;
		lst = lst->next;
	}
	tkn_free_one(previous);
}

void	tkn_free_one(t_token *tkn)
{
	if (tkn)
		free(tkn->str);
	free(tkn);
}

void	next_token(t_token **list)
{
	*list = (*list)->next;
}

////////////////////////////////////////////////////////////////////////////////
t_env	*make_ll_env(char **envp)
{
	t_env	*ptr;
	char	*key;
	char	*value;
	int		index;

	ptr = NULL;
	while (envp && *envp)
	{
		index = ft_strchr(*envp, '=') - *envp;
		key = ft_substr(*envp, 0, index);
		value = ft_substr(*envp, index + 1, ft_strlen(*envp + index));
		env_add(&ptr, key, value);
		free(key);
		free(value);
		envp++;
	}
	return (ptr);
}

void	env_add(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	**ptr;
	t_env	*new_node;

	if (!*key)
		return ;
	ptr = env;
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			break ;
		}
		ptr = &current->next;
		current = current->next;
	}
	if (current == NULL)
	{
		new_node = make_env(ft_strdup(key), ft_strdup(value), NULL);
		*ptr = new_node;
	}
}

t_env	*make_env(char *key, char *value, t_env *next)
{
	t_env	*new;

	new = (t_env *)ft_malloc(sizeof(t_env) * 1);
	new->key = key;
	new->value = value;
	new->next = next;
	return (new);
}

void	free_all_env(t_env *env)
{
	t_env	*previous;

	previous = 0;
	while (env)
	{
		free_ll_env(previous);
		previous = env;
		env = env->next;
	}
	free_ll_env(previous);
}

void	free_ll_env(t_env *env)
{
	if (env)
	{
		free(env->key);
		free(env->value);
	}
	free(env);
}

////////////////////////////////////////////////////////////////////////////////

/*int	parser(char *input, t_mnsh *shell)
{
	t_token		*list_o_tokens;
	t_token		*list_o_tokens_origin;
	t_ast_node	*ast;

	if (validate_syntax(input) == SYNTAX_ERROR)
	{
		printf("Minishell: Syntax error: %s\n", input);
		return (handle_exit_code(2));
	}
	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	lst_tkn_printer(list_o_tokens);
	list_o_tokens_origin = list_o_tokens;
	ast = build_ast(&list_o_tokens);
	lst_tknclear(&list_o_tokens_origin);
	if (DEBUG)
		print_ast(ast, 0);
	shell->ast_head = ast;
	execute_ast(shell->ast_head, shell);
	free_ast_node(shell->ast_head);
	return (handle_exit_code(-1));
}*/

//	Parsing order:
//1. Logic operators	||, &&
//2. Pipes				|
//3. Redirections		<, <<, >, >>
//4. Commands			et cetera
