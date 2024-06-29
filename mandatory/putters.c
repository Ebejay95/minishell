/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:46:51 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/28 11:05:23 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// // show lexer substruct
// void	put_ms_buffer(t_segment **segs)
// {
// 	int	i;

// 	if (segs == NULL)
// 		return ;
// 	i = 0;
// 	if (segs[i] == NULL)
// 		return ;
// 	ft_printf("[\n");
// 	while (segs[i] && segs[i]->str)
// 	{
// 		ft_printf("  [\n");
// 		ft_printf("    str: %s\n", segs[i]->str);
// 		ft_printf("    start: %d\n", segs[i]->start);
// 		ft_printf("    end: %d\n", segs[i]->end);
// 		ft_printf("    desc: %s\n", segs[i]->desc);
// 		ft_printf("  ]\n");
// 		i++;
// 	}
// 	ft_printf("]\n");
// }

// // show the lexer content for debugging
// void	put_lexer(t_lexer lexer)
// {
// 	if (lexer.is_unclosed_quote)
// 	{
// 		ft_printf("!! UNCLOSED QUOTE !!\n");
// 		return ;
// 	}
// 	ft_printf("pipe_buffer:\n");
// 	put_ms_buffer(lexer.pipe_buffer);
// 	ft_printf("quote_buffer:\n");
// 	put_ms_buffer(lexer.quote_buffer);
// 	ft_printf("dquote_buffer:\n");
// 	put_ms_buffer(lexer.dquote_buffer);
// 	ft_printf("space_buffer:\n");
// 	put_ms_buffer(lexer.space_buffer);
// 	ft_printf("option_buffer:\n");
// 	put_ms_buffer(lexer.option_buffer);
// 	ft_printf("ampersand_buffer:\n");
// 	put_ms_buffer(lexer.ampersand_buffer);
// 	ft_printf("semicolon_buffer:\n");
// 	put_ms_buffer(lexer.semicolon_buffer);
// 	ft_printf("equal_buffer:\n");
// 	put_ms_buffer(lexer.equal_buffer);
// 	ft_printf("equal_buffer:\n");
// 	put_ms_buffer(lexer.equal_buffer);
// 	ft_printf("variable_buffer:\n");
// 	put_ms_buffer(lexer.variable_buffer);
// 	ft_printf("redirection_buffer:\n");
// 	put_ms_buffer(lexer.redirection_buffer);
// 	printf("____________________________\n");
// }
