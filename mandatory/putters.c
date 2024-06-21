/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:46:51 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/21 15:25:53 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// show lexer substruct
void	put_ms_buffer(t_segment **segs)
{
	int	i;

	if (segs == NULL)
		return ;
	i = 0;
	if (segs[i] == NULL)
		return ;
	ft_printf("[\n");
	while (segs[i] != NULL)
	{
		ft_printf("  [\n");
		ft_printf("    str: %s\n", segs[i]->str);
		ft_printf("    start: %d\n", segs[i]->start);
		ft_printf("    end: %d\n", segs[i]->end);
		ft_printf("  ]\n");
		i++;
	}
	ft_printf("]\n");
}

// show the lexer content for debugging
void	put_lexer(t_lexer lexer)
{
	ft_printf("pipe_buffer:\n");
	//put_ms_buffer(lexer.pipe_buffer);
	//ft_printf("quote_buffer:\n");
	//put_ms_buffer(lexer.quote_buffer);
	//ft_printf("dquote_buffer:\n");
	//put_ms_buffer(lexer.dquote_buffer);
	//ft_printf("inrdrct_buffer:\n");
	//put_ms_buffer(lexer.inrdrct_buffer);
	//ft_printf("outrdrct_buffer:\n");
	//put_ms_buffer(lexer.outrdrct_buffer);
	//ft_printf("appoutrdrct_buffer:\n");
	//put_ms_buffer(lexer.appoutrdrct_buffer);
	//ft_printf("heredoc_buffer:\n");
	//put_ms_buffer(lexer.heredoc_buffer);
	//ft_printf("space_buffer:\n");
	//put_ms_buffer(lexer.space_buffer);
	//ft_printf("option_buffer:\n");
	//put_ms_buffer(lexer.option_buffer);
	//ft_printf("ampersand_buffer:\n");
	//put_ms_buffer(lexer.ampersand_buffer);
	//ft_printf("semicolon_buffer:\n");
	//put_ms_buffer(lexer.semicolon_buffer);
	//ft_printf("equal_buffer:\n");
	//put_ms_buffer(lexer.equal_buffer);
	//ft_printf("equal_buffer:\n");
	//put_ms_buffer(lexer.equal_buffer);
	ft_printf("variable_buffer:\n");
	put_ms_buffer(lexer.variable_buffer);
	ft_printf("redirection_buffer:\n");
	put_ms_buffer(lexer.redirection_buffer);
	printf("____________________________\n");
}
