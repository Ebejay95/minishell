/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:27:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/31 16:25:13 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	pre_parse(t_minishell *m)
{
	ft_printf("%s\n", m->prompt);
	lex_prompt(m);
}

//ft_printf(Y"AST:\n"D);
//ft_btreeput(&m->ast, put_token);
void	parse(t_minishell *m)
{
	pre_parse(m);
}
