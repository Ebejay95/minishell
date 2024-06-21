/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:46:51 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/20 11:44:31 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// show lexer substruct
void	put_ms_buffer(t_segment **segments)
{
	int	i;

	if (segments == NULL)
		return ;
	i = 0;
	if (segments[i] == NULL)
		return ;
	ft_printf("[\n");
	while (segments[i] != NULL)
	{
		ft_printf("  [\n");
		ft_printf("    str: %s\n", segments[i]->str);
		ft_printf("    start: %d\n", segments[i]->start);
		ft_printf("    end: %d\n", segments[i]->end);
		ft_printf("  ]\n");
		i++;
	}
	ft_printf("]\n");
}

// show the lexer content for debugging
void	put_lexer(t_lexer lexer)
{
	ft_printf("position: %d\n", lexer.position);
	ft_printf("is_lssthn: %d\n", lexer.is_lessthan);
	ft_printf("is_grtthn: %d\n", lexer.is_greaterthan);
	ft_printf("is_space: %d\n", lexer.is_space);
	ft_printf("is_dash: %d\n", lexer.is_dash);
	ft_printf("is_bslash: %d\n", lexer.is_bslash);
	ft_printf("is_dollar: %d\n", lexer.is_dollar);
	ft_printf("is_questmark: %d\n", lexer.is_questmark);
	ft_printf("is_ampersand: %d\n", lexer.is_ampersand);
	ft_printf("is_semicolon: %d\n", lexer.is_semicolon);
	ft_printf("is_pipe: %d\n", lexer.is_pipe);
	ft_printf("is_logical_and: %d\n", lexer.is_logical_and);
	ft_printf("is_logical_or: %d\n", lexer.is_logical_or);
	ft_printf("op_quote: %d\n", lexer.op_quote);
	ft_printf("op_dquote: %d\n", lexer.op_dquote);
	ft_printf("cl_quote: %d\n", lexer.cl_quote);
	ft_printf("cl_dquote: %d\n", lexer.cl_dquote);
	ft_printf("pipe_buffer:\n");
	put_ms_buffer(lexer.pipe_buffer);
	ft_printf("quote_buffer:\n");
	put_ms_buffer(lexer.quote_buffer);
	ft_printf("dquote_buffer:\n");
	put_ms_buffer(lexer.dquote_buffer);
	ft_printf("inrdrct_buffer:\n");
	put_ms_buffer(lexer.inrdrct_buffer);
	ft_printf("outrdrct_buffer:\n");
	put_ms_buffer(lexer.outrdrct_buffer);
	ft_printf("appoutrdrct_buffer:\n");
	put_ms_buffer(lexer.appoutrdrct_buffer);
	ft_printf("heredoc_buffer:\n");
	put_ms_buffer(lexer.heredoc_buffer);
	ft_printf("space_buffer:\n");
	put_ms_buffer(lexer.space_buffer);
	ft_printf("option_buffer:\n");
	put_ms_buffer(lexer.option_buffer);
	ft_printf("ampersand_buffer:\n");
	put_ms_buffer(lexer.ampersand_buffer);
	ft_printf("semicolon_buffer:\n");
	put_ms_buffer(lexer.semicolon_buffer);
	ft_printf("variable_buffer:\n");
	put_ms_buffer(lexer.variable_buffer);
	ft_printf("equal_buffer:\n");
	put_ms_buffer(lexer.equal_buffer);
	printf("____________________________\n");
}
