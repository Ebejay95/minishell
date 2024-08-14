/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 13:11:57 by chorst           ###   ########.fr       */
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

typedef struct s_token
{
	enum e_toktype	token;
	char			*type;
	char			*str;
	char			*expmap;
	int				had_quote;
	int				is_freed;
	union u_detail
	{
		struct s_rdct_tokdetail
		{
			char	*rdrcmeta;
			char	*rdrctarget;
		}	rdrc;
		struct s_pipe_tokdetail
		{
			int		open_prompt;
		}	pipe;
		struct s_minifile_tokdetail
		{
			int		fd;
		}	minifile;
	}	detail;
}	t_token;

// void set_tok_io_param(t_token *tok, int fdin, int fdout, const char *rdrctyp)
// {
//     if (token == NULL) return;
//     token->token = REDIRECTION;
//     token->detail.redirection.fdin = fdin;
//     token->detail.redirection.fdout = fdout;
//     token->detail.redirection.rdrcmeta = rdrcmeta;
// }

// void set_command_arg_tok_detail(t_token *token, int arglen, int is_command)
// {
//     if (token == NULL) return;
//     token->token = is_command ? COMMAND : ARGUMENT;
//     token->detail.arglen = arglen;
// }

// void set_pipe_token_details(t_token *token, int fdin, int fdout) {
//     if (token == NULL) return;
//     token->token = PIPE;
//     token->detail.pipe.fdin = fdin;
//     token->detail.pipe.fdout = fdout;
// }

typedef struct s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next;
}					t_envlst;

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
	int				pipes;
	t_list			**cmd_seqs;
	t_list			**exec_seqs;
	t_btree			*ast;
}	t_minishell;

// #############################################################################
// #                                Executer                                   #
// #############################################################################

// executer_checks.c
void	pre_exec_checks(t_minishell *m);

// executer_checks2.c
int		check_one(t_minishell *m, char *lty, char *conty);
int		check_two(t_minishell *m, char *conty, char *end);
int		check_three(t_minishell *m, t_list *curnext, char *conty);
int		check_four(t_minishell *m, char *conty, char *end);
int		check_five(t_minishell *m, t_list *curnext, char *conty);

// executer_checks3.c
int		check_six(t_minishell *m, char *end, char *conty);

// executer_command_helper.c
int		is_builtin(char *command);
void	execute_builtin(t_minishell *m, char *com, char **argv, int argc);
void	cleanup(char **argv);
int		is_word_token(t_list *node);
int		resize_argv(char ***argv, int *capacity);

// executer_command.c
void	run_command(t_minishell *m, t_list *current);

// executer_env.c
char	**own_env(t_envlst *env_lst);

// executer_helper.c
void	ft_error_exit(const char *message);
void	pic_err(t_minishell *m, int code, char *mes);
char	*get_last_cmd(t_list *ref, t_list *item);
int		add_arg(char ***argv, int *capacity, t_list *temp, int *argc);
int		add_first_arg(char **argv, t_list *current, int *argc);

// executer_helper2.c
void	reset_sequences(t_minishell *m);
char	*get_executable(t_minishell *m, char *command);
void	execute_command(t_minishell *m, char *executable, char **argv);
int		keep_for_exec(t_token *token);

// executer_inits.c
void	pre_exec_prep(t_minishell *m);
void	init_semantics(t_list *last, t_list *current);

// executer_prexecute.c
void	prexecute(t_minishell *m, t_list **tok_lst, t_list **exec_lst);

// executer_runseg.c
void	run_seg(t_minishell *m, t_list *exec_lst, int input_fd, int output_fd);

// executer.c
void	prexecute(t_minishell *m, t_list **tok_lst, t_list **exec_lst);
void	execute(t_minishell *m);

// #############################################################################
// #                                Expander                                   #
// #############################################################################

// expand_helper.c
void	ft_strfillcat(char *dest, const char *src, char fill_char);
// void		ft_strfillncat(char *dest, const char *src, size_t n, char fill_char);
char	*get_var_name_exp(const char *str, const char *expmap, size_t *pos);
char	*get_var_name(const char *str, size_t *pos);

// expand_heredoc.c
char	*expand_var(t_minishell *m, char *str, size_t *i, char **result);
char	*expand_hd(t_minishell *m, char *str);

// expand_token.c
void	expand_token(t_minishell *m, t_token *token);
void	prompt_to_token(char *prompt, t_list **tok_lst);
void	afterbreakup(t_list **tok_lst);
void	expand_toklst(t_minishell *m, t_list **tok_lst);

// expand.c
void	exp_cln(char **res, char **exp_res, char **var_name, char **exit_stats);
void	expand(t_minishell *m, char **expanded, char **expanded_map, const char *str, char *expmap, size_t start, size_t end);

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

// #############################################################################
// #                                Lexer                                      #
// #############################################################################


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
void	lex_prompt(t_minishell *m);
void	add_token_to_list(t_list **lst, t_token *token);

// minishell_helper.c
void	cleanup_minishell(t_minishell *minishell);
void	initialize_minishell(t_minishell *minishell, char **envp);

// minishell.c
int		main(int argc, char **argv, char **envp);

// parser.c
void	parse(t_minishell *m);

// pipes.c
void	split_pipes(t_minishell *m, t_list ***cmd_seq, t_list ***exec_seq);

// remove_chars.c
char	*remove_chars(const char *str, const char *chrs_to_rmv);
int		count_relevant_chars(const char *str, const char *chrs_to_rmv);
void	remove_helper(const char *s, const char *chrs_to_rmv, char *new_s);

// whitespace_handler.c
char	*whitespace_handler(const char *str);

// tokens.c
t_token	*create_token(char *str, char *expmap);
char	*toktype_to_str(enum e_toktype token);
void	put_token(void *content);
void	update_tok_type(t_token *tok, enum e_toktype token);
void	update_tok_type_next(t_list *current, enum e_toktype token);
void	update_tok_type_next_word(t_list *current, enum e_toktype token);
void	free_token(void *n);
// signal.c
void	handle_child_process(int sig);
void	handle_main_process(int sig);
void	setup_signals(t_minishell *minishell);
void	reset_signals(void);
void	handle_heredoc_signal(int sig);

// #############################################################################
// #                               Builtins                                    #
// #############################################################################

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
void	ft_exit(char **argv, int *exitcode);

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

#endif
