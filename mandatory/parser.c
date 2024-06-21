/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:27:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/21 15:24:44 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	parse(t_minishell *m)
{
	ft_printf("\x1b[36m%s\x1b[0m\n", m->prompt);
	put_lexer(m->lexer);
	//t_token		*expipeone;
	//expipeone = create_token(PIPE);
	//ast_add(&minishell.ast, minishell.ast, "next", expipeone);
	ft_btreeput(&m->ast, put_token);
}
