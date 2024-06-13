/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/13 09:25:34 by chorst           ###   ########.fr       */
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

typedef struct	s_segment
{
	int			position;
	int			end;
	char		*quote;
}	t_segment;

typedef struct	s_arglexer
{
	int			position;
	int			is_lssthn;
	int			is_grtthn;
	int			is_space;
	int			is_dash;
	int			is_bslash;
	int			is_dollar;
	int			is_questmark;
	int			op_quote;
	int			op_dquote;
	bool		open_quote;
	t_segment	**pipe_buffer;
	t_segment	**quote_buffer;
	t_segment	**dquote_buffer;
	t_segment	**lssgrt_buffer;
	t_segment	**space_buffer;
	t_segment	**option_buffer;
	t_segment	**variable_buffer;
	int			cl_quote;
	int			cl_dquote;
}	t_arglexer;

#endif
