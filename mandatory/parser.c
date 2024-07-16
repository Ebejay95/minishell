/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:27:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/10 09:05:25 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	pre_parse(t_minishell *m)
{
	ft_printf("%s\n", m->prompt);
	lex_prompt(m);
	//put_lexer(m->lexer);
	//ft_expand(&m->prompt, m->lexer, 0);
	//ft_expand(&m->prompt, m->lexer, 22);
}

void	parse(t_minishell *m)
{
	pre_parse(m);
	//ft_printf("\x1b[36m%s\n", m->prompt);
	//lex_prompt(m);
	//put_lexer(m->lexer);
	//ft_printf("\x1b[0m\n");
	//t_token		*expipeone;
	//expipeone = create_token(PIPE);
	//ast_add(&minishell.ast, minishell.ast, "next", expipeone);
	ft_printf(Y"AST:\n"D);
	ft_btreeput(&m->ast, put_token);
}
