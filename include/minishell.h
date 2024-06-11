/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:18:56 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/11 20:33:09 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./../libft/libft.h"

typedef struct	s_arglexer
{
	int		position;
	int		is_space;
	int		is_dash;
	int		is_bslash;
	int		op_quote;
	int		op_dquote;
	int		cl_quote;
	int		cl_dquote;
	int		is_option;
	char	*options;
}	t_arglexer;

#endif