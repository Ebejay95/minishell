/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 09:27:06 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Librarys*/
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./../libft/libft.h"
# include <limits.h>
# include <pthread.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <sys/stat.h>
# include <sys/wait.h>

// Define Colours
# define R "\033[0;31m"
# define G "\033[0;32m"
# define B "\033[0;34m"
# define Y "\033[0;93m"
# define C "\033[0;36m"
# define M "\033[0;35m"
# define W "\033[0;97m"
# define D "\033[0m"

// Define Error Messages
# define AD_C_ON_N "Cannot create new %s on empty parent - nothing modified\n"
# define AD_N_ON_N "Cannot create new %s on empty sibling - nothing modified\n"
# define B_NO_DEF "Define branch (child|next) - nothing modified\n"
# define AD_C_N_AL "Nothing modified on child %s for parent %s not allowed\n"
# define AD_N_N_AL "Nothing modified on next %s for sibling %s not allowed\n"
# define RT_N_AL "Root token %s not allowed\n"
# define CT_N_AL "Child token %s for parent %s not allowed\n"
# define NT_N_AL "Next token %s for sibling %s not allowed\n"

// Syntax errors
# define SYN_ERR_NEWLINE "bash: syntax error near unexpected token `newline'"
# define SYN_ERR_PIPE "bash: syntax error near unexpected token `|'"
# define SYN_ERR_GT "bash: syntax error near unexpected token `>'"
# define SYN_ERR_DBL_GT "bash: syntax error near unexpected token `>>'"
# define SYN_ERR_LT "bash: syntax error near unexpected token `<'"
# define SYN_ERR_DBL_LT "bash: syntax error near unexpected token `<<'"

// General errors
# define RDRC "Redirection"
# define PIP "Pipe"
# define NO_FILE_NAME "No file name specified for redirection\n"
# define OPEN_FILE_ERROR "Failed to open file for writing (truncate/append)"
# define NUMERIC_ARG_REQUIRED "numeric argument required"
# define MEM_ERR "Memory allocation failed\n"
# define ERR_OLDPWD "🍕🚀🌈🦄🍺: cd: OLDPWD not set\n"

// Sets
# define ALPH_UP "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define ALPH_LW "abcdefghijklmnopqrstuvwxyz"
# define NUMS "0123456789"
# define WHTSPC " \t\n\v\f\r"
# define SPCCHR "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
# define VS "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
# define RDRCSET "><0123456789"

// mode
# define DEBUG 0
# define DEBUG_LOG "/tmp/minishell_debug.log"
# define MAXPIPS 1024

// #############################################################################
// #                        the one and only g_global                          #
// #############################################################################

extern int	g_global;

// #############################################################################
// #                                 Enums                                     #
// #############################################################################

typedef enum e_toktype
{
	REDIRECTION,
	PIPE,
	COMMAND,
	WORD,
	MINIFILE,
	DELIMITER,
	UNSET
}	t_toktype;

// #############################################################################
// #                               Structures                                  #
// #############################################################################

typedef struct s_tokenizer_state
{
	char	*ptr;
	char	*current_token;
	char	*expmap;
	int		current_pos;
	int		quote_level;
	int		escape_next;
	int		current_token_size;
}	t_tokenizer_state;

typedef struct s_token
{
	enum e_toktype	token;
	char			*str;
	char			*expmap;
	int				had_quote;
	int				is_freed;
	char			*rdrcmeta;
	char			*rdrctarget;
	int				fd;
}	t_token;

typedef struct s_temps
{
	char	*temp;
	char	*temp1;
	char	*temp2;
	char	*temp3;
	char	*temp4;
	char	*expmap;
	char	*filecontent;
	char	*line;
}	t_temps;

typedef struct s_fd
{
	int	input;
	int	output;
	int	last_input;
	int	last_output;
}	t_fd;

typedef struct s_pipe_data
{
	t_list	***cmd_seq;
	t_list	**s;
	int		*count;
	int		*pipes;
}	t_pipe_data;

typedef struct s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next;
}					t_envlst;

typedef struct s_pipe_info
{
	int				pipe_fd[2];
	int				prv_pipe;
	int				i;
	int				total;
}					t_pipe_info;

typedef struct s_minishell
{
	char			*temp1;
	char			*temp2;
	char			*temp3;
	char			*temp4;
	char			**envp;
	t_envlst		*env_list;
	char			*prompt;
	int				is_interactive;
	int				leave;
	int				modus;
	int				last_exitcode;
	int				exitcode;
	int				pid;
	int				in_heredoc;
	t_list			*tok_lst;
	t_list			*exec_lst;
	pid_t			*pids;
	int				pipes;
	int				argc;
	char			*args[1024];
	t_list			*cmd_seqs[1024];
	t_list			*exec_seqs[1024];
	t_btree			*ast;
}	t_minishell;

typedef struct s_exp {
	char		*result;
	char		*expmap_result;
	char		*str;
	char		*expmap;
	size_t		start;
	size_t		end;
	t_minishell	*m;
	t_token		*token;
}	t_exp;

typedef struct s_expand_ctx
{
	t_minishell	*m;
	char		**result;
	char		**expmap_result;
	const char	*str;
	char		*expmap;
	size_t		*i;
	size_t		var_start;
}	t_expand_ctx;

typedef struct s_exp_p
{
	t_minishell	*m;
	char		**expanded;
	char		**expanded_map;
	const char	*str;
	char		*expmap;
	size_t		start;
	size_t		end;
}	t_exp_p;

typedef struct s_memory_info {
	char	*temp;
	char	*expmap_temp;
	size_t	result_len;
	size_t	expmap_len;
	size_t	value_len;
}	t_memory_info;

typedef struct s_exp_data {
	char	*expanded;
	char	*expanded_map;
	char	*temp;
	char	*expmap_temp;
}	t_exp_data;

typedef struct s_expand_data
{
	char			**res;
	char			**exp_res;
	const char		*str;
	char			*expmap;
	size_t			*i;
	int				*escaped;
	t_exp_p			*params;
}	t_expand_data;

// #############################################################################
// #                               Builtins                                    #
// #############################################################################
void	reset_minishell_args(t_minishell *m);
void	mlstclear(t_list *list);
void	ft_envlstclear(t_minishell *minishell);
// cd.c
void	ft_cd(int argc, char **argv, t_envlst **envp);
void	cd_home(t_envlst **envp);
void	cd_oldpwd(t_envlst **envp);

// echo.c
void	ft_echo(char **args);
int		handle_options(char **args, bool *newline, int start_index);
void	print_output(char **args, int start_index, bool newline);

// env.c
void	ft_env(t_envlst *env_list); // works

// exit.c
void	ft_exit(char **argv, t_minishell *m);

// export.c
void	ft_export(int argc, char **argv, t_envlst **envp);
void	my_export(t_envlst **envp, char **argv);
void	change_env(t_envlst **env_lst, char *key, char *value, int free_it);
void	sort_envp(char **envp);
char	**copy_envp(t_envlst *envp);

// pwd.c
void	ft_pwd(char **args);

// set.c
void	change_var_value(t_envlst **envp, char **argv);
void	upgrade_var_value(t_envlst **envp, char *argv);
void	update_var_value(t_envlst **envp, char *argv);

// unset.c
void	ft_unset(t_envlst **envp, char **argv);
void	free_envlst_node(t_envlst *node);

// var_helper.c
int		count_list(t_envlst *envp);
void	print_env_variable(const char *env_var);
int		is_var_name(t_envlst *envp, char **argv);
char	*my_getenv(const char *name, t_envlst *envp);
char	*ft_strndup(const char *s, size_t n);

// #############################################################################
// #                                Executer                                   #
// #############################################################################

// executer_checks.c
void	pre_exec_checks(t_minishell *m);
t_token	*duplicate(t_token *src);

// executer_checks2.c
int		check_one(t_minishell *m, enum e_toktype lty, enum e_toktype conty);
int		check_two(t_minishell *m, enum e_toktype conty, enum e_toktype end);
int		check_three(t_minishell *m, t_list *curnext, enum e_toktype conty);
int		check_four(t_minishell *m, enum e_toktype conty, enum e_toktype end);
int		check_five(t_minishell *m, t_list *curnext, enum e_toktype conty);

// executer_checks3.c
int		check_six(t_minishell *m, enum e_toktype end, enum e_toktype conty);

// executer_command.c
int		is_builtin(char *command);
void	execute_builtin(t_minishell *m, char *com, char **argv, int argc);
void	cleanup(char **argv);
int		is_word_token(t_list *node);
int		resize_argv(char ***argv, int *capacity);

// executer_env.c
char	**own_env(t_envlst *env_lst);

// executer_helper.c
void	ft_error_exit(const char *message);
void	pic_err(t_minishell *m, int code, char *mes);
char	*get_last_cmd(t_list *ref, t_list *item);
int		add_arg(char ***argv, int *capacity, t_list *temp, int *argc);
int		add_first_arg(char **argv, t_list *current, int *argc);

// executer_helper2.c
char	*get_executable(t_minishell *m, char *command);
void	execute_command(t_minishell *m, char *executable, char **argv);
int		keep_for_exec(t_token *token);

// executer_inits.c
void	pre_exec_prep(t_minishell *m);
void	init_semantics(t_list *last, t_list *current);
void	init_fd(t_fd *fd, int input_fd, int output_fd);

// executer_pipes.c
void	execute_with_pipes(t_minishell *m);

// executer_prexecute.c
void	prexecute(t_minishell *m, int i);

// executer_redirection.c
void	run_in_redirection(t_token *token, t_fd *fd);
void	run_out_redirection(t_token *token, t_fd *fd);
void	run_redirection(t_token *token, t_fd *fd);

// executer_runseg_helper.c
void	add_argument(t_minishell *m, char *arg);
void	exec_builtin_cmd(t_minishell *m, char **args, int arg_count, t_fd *fd);
void	execute_external_command(t_minishell *m, char **args, t_fd *fd);
void	process_tok(t_minishell *m, t_list *exec_lst, t_fd *fd);

// executer_runseg.c
void	run_heredoc(t_token *t, t_fd *fd);
void	run_command(t_minishell *m, char **args);
void	run(t_minishell *m, char **args, int arg_count, t_fd *fd);
void	cleanup_fds(t_fd *fd);
void	run_seg(t_minishell *m, int i, int input_fd, int output_fd);

// executer_signals.c
void	run_child_process(t_minishell *m, t_pipe_info *pi);
void	handle_parent_process(t_pipe_info *pi);
void	wait_for_children(t_minishell *m, t_pipe_info *pi);
void	setup_child_process(t_fd *fd);
void	run_parent_process(t_minishell *m, pid_t pid);

// executer.c
void	newfree_token(t_token *token);
int		allocate_pids(t_minishell *m, int pipes);
int		fork_and_execute(t_minishell *m, t_pipe_info *pi);
void	execute(t_minishell *m);

// #############################################################################
// #                                Expander                                   #
// #############################################################################

// expand_helper.c
void	ft_strfillcat(char *dest, const char *src, char fill_char);
char	*get_var_name_exp(const char *str, const char *expmap, size_t *pos);
char	*get_var_name(const char *str, size_t *pos);
int		handle_no_var_name(t_expand_ctx *ctx);
int		allocate_memory(t_memory_info *m);
void	copy_and_concatenate(t_expand_ctx *ctx, t_memory_info *m, char *v);
void	update_expmap(t_expand_ctx *ctx, char *expmap_temp, char *var_value);
void	update_and_free(t_expand_ctx *ctx, t_memory_info *mem_info);
int		handle_var_value(t_expand_ctx *ctx, char *var_value);
int		handle_variable(t_expand_ctx *ctx);
void	init_exp(t_exp *exp);
void	handle_unexpanded_part(t_exp *exp);
void	init_exp_data(t_exp_data *data);
void	setup_expand_params(t_exp *exp, t_exp_p *params, t_exp_data *data);
int		allocate_temp_memory(t_exp *e, t_exp_data *d);
int		allocate_expmap_temp_memory(t_exp *exp, t_exp_data *data);
void	update_result(t_exp *exp, t_exp_data *data);
void	update_expmap_result(t_exp *exp, t_exp_data *data);
void	cleanup_expansion_data(t_exp_data *data);
void	handle_expanded_part(t_exp *exp);
void	process_token(t_exp *e);
void	expand_token(t_minishell *m, t_token *token);
int		expmapcheck(char *expmap, const char *str, int i, int escaped);
void	expcn(char **res, char **exp_res, char **var_name, char **exit_stats);
int		initialize_expansion(t_expand_data *d);
int		handle_dollar_sign(t_expand_data *data, t_expand_ctx *ctx);
int		handle_escape_character(t_expand_data *d);
int		append_regular_character(t_expand_data *data);
int		process_character(t_expand_data *data, t_expand_ctx *ctx);
void	finalize_expansion(t_expand_data *data);
void	expand(t_exp_p *p, t_expand_ctx *ctx);
void	free_expmap_temp_memory(t_exp_data *data);
void	free_temp_memory(t_exp_data *d);

// expand_heredoc.c
char	*expand_var(t_minishell *m, char *str, size_t *i, char **result);
char	*expand_hd(t_minishell *m, char *str);

// expand_token.c
void	expand_token(t_minishell *m, t_token *token);
void	afterbreakup(t_list **tok_lst);
void	expand_toklst(t_minishell *m, t_list **tok_lst);

// expand.c
void	expcn(char **res, char **exp_res, char **var_name, char **exit_stats);
void	expand(t_exp_p *params, t_expand_ctx *ctx);

// #############################################################################
// #                               Handler                                     #
// #############################################################################

// handle_exit_status.c
int		handle_exit_status(t_minishell *m, char **res, char **expmap_res);

// handle_exist_status2.c
int		are_exitinputs_valid(t_minishell *m, char **res, char **expmap_res);
char	*get_exit_status_str(t_minishell *m);

// handle_escape.c
int		are_inputs_valid(char **result, char **expmap_result, int *escaped);
int		handle_first_esc(int *escaped);
int		handle_second_esc(char **result, char **expmap_result, int *escaped);
int		allocate_new_buf(char **t, char **et, const char *r, const char *er);
void	append_esc_character(char *t, char *e, const char *r, const char *er);
int		handle_escape(char **result, char **expmap_result, int *escaped);

// handle_heredoc.c
void	handle_heredoc(t_minishell *m, t_list *current);

// handle_infile.c
void	handle_infile(t_list *current);

// handle_trunc_append.c
void	handle_trunc_append(t_list *current);
t_token	*get_next_content(t_list *current);

// handle_variable.c
int		handle_variable(t_expand_ctx *ctx);

// handle_trunc_append2.c
t_token	*get_next_content(t_list *current);
void	handle_allocation_error(char *filecontent, char *line);
char	*add_line(char *cont, char *tmp, const char *line, size_t total_size);

// #############################################################################
// #                                Lexer                                      #
// #############################################################################

// #############################################################################
// #                                Tokens                                     #
// #############################################################################

// tokens.c
int		validate_input(const char *str, const char *expmap);

// tokens2.c
void	free_if_not_null(void	**ptr);
void	free_token(void	*n);
void	update_tok_type(t_token *tok, enum e_toktype token);
void	update_tok_type_next_word(t_list *current, enum e_toktype token);
void	update_tok_type_next(t_list *current, enum e_toktype token);

// tokens3.c
void	free_token_resources(t_token *newtok);
t_token	*create_token(char *str, char *expmap);
void	print_toktype(enum e_toktype token);

// tokens4.c
void	put_token(void	*content);
t_token	tok_lst_get(void	*n);

// #############################################################################
// #                          Mandatory Functions                              #
// #############################################################################

// ast.c
void	ast_add(t_btree **ast, t_btree *cur, char *branch, t_token *tok);

// hierarchy_validation.c
int		vd_null_add(t_btree *ast, t_token *newtok);
int		check_child_rel(t_token *current, t_token *new);
int		check_next_rel(t_token *current, t_token *new);
int		vd_tree_add(t_btree *current, char *branch, t_token *newtok);

// init_envlst.c
void	init_env_list(char **envp, t_minishell *m);
void	extract_name_value(char *arg, char **name, char **value);
void	add_env_node(t_envlst **env_list, char *name, char *value);

// lexer.c
void	add_new_abtoken(t_list **current, char *word, char *expmap);
void	process_abtoken(t_list **current, t_token *cur);
int		should_process_token(t_token *t);
void	afterbreakup(t_list **tok_lst);
void	detect_lexing_errors(t_minishell *m);
void	add_token_to_list(t_list **lst, t_token *token);
void	handle_escape_char(t_tokenizer_state *state);
void	handle_backslash(t_tokenizer_state *state);
void	handle_single_quote(t_tokenizer_state *state);
void	handle_double_quote(t_tokenizer_state *state);
void	handle_space(t_minishell *m, t_tokenizer_state *state);
void	handle_pipe(t_minishell *m, t_tokenizer_state *state);
void	handle_current_token(t_minishell *m, t_tokenizer_state *state);
void	create_rdrct_token(t_minishell *m, char *rdrct, char *expmap);
void	handle_double_redirection(t_minishell *m, t_tokenizer_state *state);
void	handle_single_redirection(t_minishell *m, t_tokenizer_state *state);
void	handle_redirection(t_minishell *m, t_tokenizer_state *state);
void	handle_regular_char(t_tokenizer_state *state);
void	resize_token_buffers(t_tokenizer_state *state);
void	init_tokenizer_state(t_tokenizer_state *state, char *p);
void	process_char(t_minishell *m, t_tokenizer_state *s);
void	cleanup_tokenizer_state(t_tokenizer_state *state);
void	prompt_to_token(t_minishell *m);
void	expand_toklst(t_minishell *m, t_list **tok_lst);
void	lex_prompt(t_minishell *m);

// minishell_helper.c
void	cleanup_minishell(t_minishell *minishell);
void	initialize_minishell(t_minishell *minishell, char **envp);

// minishell.c
int		main(int argc, char **argv, char **envp);

// parser.c
void	parse(t_minishell *m);

// pipes.c
void	split_pipes(t_minishell *m);

// remove_chars.c
char	*remove_chars(const char *str, const char *chrs_to_rmv);
int		count_relevant_chars(const char *str, const char *chrs_to_rmv);
void	remove_helper(const char *s, const char *chrs_to_rmv, char *new_s);

// whitespace_handler.c
char	*whitespace_handler(const char *str);

// signal.c
void	handle_child_process(int sig);
void	handle_main_process(int sig);
void	setup_signals(t_minishell *minishell);
void	reset_signals(void);
void	handle_heredoc_signal(int sig);

#endif
