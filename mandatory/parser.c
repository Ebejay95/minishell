/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:27:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/19 01:16:09 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	pre_parse(t_minishell *m)
{
	ft_printf("%s\n", m->prompt);
	lex_prompt(m);
}

void	parse(t_minishell *m)
{
	pre_parse(m);
	ft_printf(Y"AST:\n"D);
	ft_btreeput(&m->ast, put_token);
}
