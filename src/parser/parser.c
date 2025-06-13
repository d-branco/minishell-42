/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/13 15:40:06 by abessa-m         ###   ########.fr       */
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
static int	get_tkn_type(char *str);
const char	*get_tkn_as_str(enum e_token_type n);
t_token		*make_one_tkn(t_token *next, char *str, enum e_token_type type);
int			get_str_token(char **word, char **str);
int			handle_quote(char *c, int *in_s_qts, int *in_d_qts);

int			parse_ast(t_ast **ast, t_token **tkn);
int			parse_tokens(t_ast **ast, t_token **tkn);
static int	parse_conditionnal(t_ast **ast, t_token **tkn);

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
char		*dollar_expansion(char **str, t_env *env, int retn, int state);
char		*expand_variable(char **str, t_env *env, int state);
char		*ret_env_key(t_env *env, char *key);
void		insert_value(char **buf, char *val, int pos, int extra_space);

int	parse_n_exec_input(char *input, t_mnsh *shell)
{
	t_token			*lst_tkn;
	t_token			*lst_tkn_origin;
	t_ast			*ast;
	static t_env	*ll_env[1];

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
}

////////////////////////////////////////////////////////////////////////////////
char	*param_expansion(char *str, t_env *env, int retn)
{
	char	*val;
	char	*res;
	int		i;
	int		in_s_qts;
	int		in_d_qts;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	in_s_qts = 0;
	in_d_qts = 0;
	i = 0;
	while (*str)
	{
		if (*str == '$' && !in_s_qts)
		{
			val = dollar_expansion(&str, env, retn, in_d_qts);
			insert_value(&res, val, i, ft_strlen(str) + 1);////////////////
			i += ft_strlen(val);
			free(val);
		}
		else
		{
			handle_quote(str, &in_s_qts, &in_d_qts);
			res[i++] = *(str++);
		}
	}
	res[i] = 0;
	return (res);
}

char	*dollar_expansion(char **str, t_env *env, int retn, int state)
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

char	*expand_variable(char **str, t_env *env, int state)
{
	char	*ret;
	char	*key;
	char	*value;
	int		i;

	i = 1;
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
		ret = value;
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

void	insert_value(char **buf, char *val, int pos, int extra_space)
{
	int		len;
	char	*tmp;

	(*buf)[pos] = 0;
	len = ft_strlen(*buf) + ft_strlen(val) + extra_space;
	tmp = ft_malloc(1 * len);
	ft_strlcpy(tmp, *buf, len);
	ft_strlcat(tmp, val, len);
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
	t_tube	*new_chunk;
	t_tube	*res;
	t_tube	**ptr;
	int		exp;

	exp = 0;
	res = NULL;
	ptr = &res;
	current = *cmd;
	while (current)
	{
		if (expand_tube(current, &new_chunk, env, error_code) != 0)
			exp = -1;
		*ptr = new_chunk;
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
		expand_wildcards(res, tubo);
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
	int		i;
	char	*res;
	int		in_s_qts;
	int		in_d_qts;

	res = ft_malloc(1 * (ft_strlen(str) + 1));
	in_s_qts = 0;
	in_d_qts = 0;
	i = 0;
	while (*str)
	{
		if (!handle_quote(str, &in_s_qts, &in_d_qts))
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
		error_code = launch_builtin(exec, 0);
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
	while (1)
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

static int	parse_conditionnal(t_ast **ast, t_token **tkn)
{
	t_list	*pipeline;

	*ast = NULL;
	if ((*tkn)->type == e_PARENTHESIS_OPEN)
	{
		next_token(tkn);
		if (parse_tokens(ast, tkn) != SUCCESS)
			return (SYNTAX_ERROR);
		if (assert_token(*tkn, e_PARENTHESIS_CLOSE) != SUCCESS)
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

static int	get_tkn_type(char *str)
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
	static const char	*tok_strings[9] = {"(", ")", "&&", "||",
		"<<", "<", ">>", ">", "|"};

	if (n == e_END)
		return ("EOL");
	if (n > 9 || n < 0)
		return (NULL);
	return (tok_strings[n]);
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

//	in_s_qts means: in single quotes
//	in_d_qts means: in double quotes
int	get_str_token(char **word, char **str)
{
	int	i;
	int	in_s_qts;
	int	in_d_qts;

	in_s_qts = 0;
	in_d_qts = 0;
	i = 0;
	while ((*str)[i] && (!ft_isspace((*str)[i]) || (in_s_qts + in_d_qts)))
	{
		handle_quote(*str + i, &in_s_qts, &in_d_qts);
		if (!(in_s_qts + in_d_qts) && get_tkn_type(*str + i) != e_WORD)
			break ;
		i++;
	}
	*word = ft_substr(*str, 0, i);
	*str += i;
	if (in_s_qts || in_d_qts)
	{
		ft_putstr_fd("Minishell: syntax error\n", STDERR_FILENO);
		return (handle_exit_code(2));
	}
	return (0);
}

int	handle_quote(char *c, int *in_s_qts, int *in_d_qts)
{
	if (*c == '\'' && !(*in_d_qts))
		*in_s_qts = !(*in_s_qts);
	else if (*c == '"' && !(*in_s_qts))
		*in_d_qts = !(*in_d_qts);
	else
		return (0);
	return (1);
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
