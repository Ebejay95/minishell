/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/11 12:16:04 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Librarys
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

// Sets
# define ALPH_UP "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define ALPH_LW "abcdefghijklmnopqrstuvwxyz"
# define NUMS "0123456789"
# define WHTSPC " \t\n\v\f\r"
# define SPCCHR "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
# define VS "$_=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
# define RDRCSET "><0123456789"

// #############################################################################
// #                                 Union                                     #
// #############################################################################



// #############################################################################
// #                                 Enums                                     #
// #############################################################################

typedef enum e_toktype
{
	REDIRECTION,
	PIPE,
	COMMAND,
	ARGUMENT,
	UNSET
}	t_toktype;

// #############################################################################
// #                               Structures                                  #
// #############################################################################

typedef struct s_segment
{
	char			*str;
	enum e_toktype	type;
}					t_segment;

typedef struct s_token // USE UNION!!!!!
{
	enum e_toktype	token;
	char			*type;
	char			*str;
	int				start;
	int				end;
	int				expand;
}	t_token;

typedef struct s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next;
}					t_envlst;

typedef struct s_minishell
{
	char		**envp;
	char		*prompt;
	t_list		*tok_lst;
	t_btree		*ast;
}	t_minishell;

// #############################################################################
// #                          Mandatory Functions                              #
// #############################################################################

// ast.c
void		ast_add(t_btree **ast, t_btree *cur, char *branch, t_token *tok);

// hierarchy_validation.c
int			vd_null_add(t_btree *ast, t_token *newtok);
int			check_child_rel(t_token *current, t_token *new);
int			check_next_rel(t_token *current, t_token *new);
int			vd_tree_add(t_btree *current, char *branch, t_token *newtok);

// expand.c
//void		ft_expand(char **prompt, t_lexer lexer, int start_index);

// lexer.c
void		lex_prompt(t_minishell *m);
// t_segment	*find_seg_by_start(t_segment **segs, int search);
// t_segment	*find_seg_by_end(t_segment **segs, int search);
// t_segment	*find_seg_by_str(t_segment **segs, char *search);

// minishell.c
void		execute_command(char *prompt, t_envlst **envlst);
void		extract_name_value(char *env_var, char **name, char **value);
void		add_env_node(t_envlst **env_list, char *name, char *value);
t_envlst	*init_env_list(char **envp);

// parser.c
void		parse(t_minishell *m);

// putters.c

// remove_chars.c
char		*remove_chars(char *str, const char *chars_to_remove);
int			count_relevant_chars(const char *str, const char *chars_to_count);
void		remove_helper(char *str, const char *chars_to_remove, char *new_str);

// segments_helper.c
// char		*build_segment(int start, int end, const char *prompt);
t_segment	**seg_clear_all(int idx, t_segment **segs);
// t_segment	**build_segments(char const *prompt, char *type, t_segment **segs);

// segments.c
t_segment	**lex_redirections(char *str);
t_segment	**lex_pipes(char *str);
t_segment	**get_segs(char *prompt, char *type);
t_segment	**get_quote_segs(t_minishell *m, char type);
t_segment	**get_oc_segs(t_minishell *m, char mode, char *open, char *close);
t_segment	**get_set_segs(t_minishell *m, char *set);

// tokens.c
t_token		*create_token(char *str, int start, int end, int expand);
char		*toktype_to_str(enum e_toktype token);
void		put_token(void *content);
void		update_tok_type(t_token *tok, enum e_toktype token);

// signal.c
void		handle_signal(int sig);
void		setup_signals(void);

// #############################################################################
// #                               Builtins                                    #
// #############################################################################

// cd.c
void		ft_cd(int argc, char **argv, t_envlst ***envp);
void		cd_home(t_envlst ***envp);

// echo.c
void		ft_echo(char **args);

// env.c
void		ft_env(t_envlst *env_list); // works

// exit.c
void		ft_exit(char **argv);

// export.c
void		ft_export(int argc, char **argv, t_envlst ***envp);
void		my_export(t_envlst **envp, char **argv);
void		change_env_node(t_envlst **env_list, char *name, char *value, int free_it);
void		sort_envp(char **envp);
char		**copy_envp(t_envlst *envp);

// pwd.c
void		ft_pwd(char **args);

// set.c
void		change_var_value(t_envlst **envp, char **argv);
void		upgrade_var_value(t_envlst **envp, char *argv);
void		update_var_value(t_envlst **envp, char *argv);

// unset.c
void		ft_unset(t_envlst **envp, char **argv);
void		free_envlst_node(t_envlst *node);

// var_helper.c
int			count_env_list(t_envlst *envp);
void		print_env_variable(const char *env_var);
int			is_var_name(t_envlst *envp, char **argv);
char		*ft_strndup(const char *s, size_t n);

// #############################################################################
#endif
