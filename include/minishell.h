/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/18 13:05:28 by chorst           ###   ########.fr       */
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


// #############################################################################
// #                               Structures                                  #
// #############################################################################

typedef struct s_segment
{
	char		*str;
	int			start;
	int			end;
}	t_segment;

typedef struct s_lexer
{
	int			is_unclosed_quote;
	int			position;
	int			is_lessthan;
	int			is_greaterthan;
	int			is_space;
	int			is_dash;
	int			is_bslash;
	int			is_dollar;
	int			is_questmark;
	int			is_ampersand;
	int			is_semicolon;
	int			is_pipe;
	int			is_logical_and;
	int			is_logical_or;
	int			op_quote;
	int			op_dquote;
	int			cl_quote;
	int			cl_dquote;
	t_segment	**pipe_buffer;
	t_segment	**quote_buffer;
	t_segment	**dquote_buffer;
	t_segment	**inrdrct_buffer;
	t_segment	**outrdrct_buffer;
	t_segment	**appoutrdrct_buffer;
	t_segment	**heredoc_buffer;
	t_segment	**space_buffer;
	t_segment	**option_buffer;
	t_segment	**ampersand_buffer;
	t_segment	**semicolon_buffer;
	t_segment	**variable_buffer;
	t_segment	**equal_buffer;
}	t_lexer;

typedef struct s_parser
{
	int			parse_pos;
	char		parse_char;
}	t_parser;

typedef struct s_minishell
{
	char		**envp;
	char		**refenvp;
	char		*prompt;
	t_lexer		lexer;
	t_parser	parser;
}	t_minishell;

// #############################################################################
// #                          Mandatory Functions                              #
// #############################################################################

// minishell.c
void		parse_table(t_minishell *minishell);
void		lex_prompt(t_minishell *minishell);
char		*input_cleaner(char *prompt);
void		execute_command(char *prompt, char ***envp);

// putters.c
void		put_ms_buffer(t_segment **segments);
void		put_lexer(t_lexer lexer);

// segments.c
t_segment	**get_segments(char *prompt, char *type);
t_segment	**get_quote_segments(t_minishell *minishell, char type);

// segments_helper.c
char		*build_segment(int start, int end, const char *prompt);
t_segment	**seg_clear_all(int idx, t_segment **segments);
t_segment	**build_segments(char const *prompt, char *type, t_segment **segments);


// #############################################################################
// #                               Builtins                                    #
// #############################################################################

// cd.c
void		ft_cd(char **envp, char *args);

// echo.c
void		ft_echo(char **args);

// env.c
void		ft_env(char ***args);

// exit.c
void		ft_exit(char **args);

// export.c
void		ft_export(int argc, char **argv, char ****envp);
char		**copy_envp(char **envp);
char		**sort_envp(char ***envp);
void		add_export(char **argv, char *****envp);
void		free_it(char **str);

// pwd.c
void		ft_pwd(char **args);

// unset.c
void		ft_unset(char **envp, const char *name);

#endif
