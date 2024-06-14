/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/14 12:22:41 by chorst           ###   ########.fr       */
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
	char		*prompt;
	t_lexer		lexer;
	t_parser	parser;
}	t_minishell;

void		ft_pwd(void);
void		put_lexer(t_lexer lexer);
char		*build_segment(int start, int end, const char *prompt);
t_segment	**seg_clear_all(int idx, t_segment **segments);
t_segment	**build_segments(char const *prompt, char *type, t_segment **segments);
t_segment	**get_quote_segments(t_minishell *minishell, char type);
t_segment	**get_segments(char *prompt, char *type);
void		put_ms_buffer(t_segment **segments);

// Builtins
void		builtin_echo(char **args);
void		builtin_cd(char **args);
void		builtin_pwd(char **args);
// void		builtin_export(char **args);
void		builtin_unset(char **args);
void		builtin_env(char **args);
// void		builtin_exit(char **args);

#endif
