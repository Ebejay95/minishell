/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/13 11:59:43 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_putallenv(char **env)
{
	int		i;
	char	*print;

	i = 0;
	while (env[i] != 0)
	{
		print = ft_color(env[i], YELLOW);
		ft_printf(print);
		ft_printf("\n");
		free(print);
		i++;
	}
}

void	lex_prompt(t_minishell *minishell)
{
	ft_printf("\x1b[36m%s\x1b[0m\n", minishell->prompt);
	minishell->lexer.position = 0;
	minishell->lexer.is_lessthan = 0;
	minishell->lexer.is_greaterthan = 0;
	minishell->lexer.is_space = 0;
	minishell->lexer.is_dash = 0;
	minishell->lexer.is_bslash = 0;
	minishell->lexer.is_dollar = 0;
	minishell->lexer.is_questmark = 0;
	minishell->lexer.is_ampersand = 0;
	minishell->lexer.is_semicolon = 0;
	minishell->lexer.is_pipe = 0;
	minishell->lexer.is_logical_and = 0;
	minishell->lexer.is_logical_or = 0;
	minishell->lexer.op_quote = 0;
	minishell->lexer.op_dquote = 0;
	minishell->lexer.cl_quote = 0;
	minishell->lexer.cl_dquote = 0;
	minishell->lexer.pipe_buffer = get_segments(minishell->prompt, "|");
	minishell->lexer.quote_buffer = get_oc_segments(minishell->prompt, "'");
	minishell->lexer.dquote_buffer = get_oc_segments(minishell->prompt, "\"");
	minishell->lexer.inrdrct_buffer = get_segments(minishell->prompt, "<");
	minishell->lexer.outrdrct_buffer = get_segments(minishell->prompt, ">");
	minishell->lexer.appoutrdrct_buffer = get_segments(minishell->prompt, ">>");
	minishell->lexer.heredoc_buffer = get_segments(minishell->prompt, "<<");
	minishell->lexer.space_buffer = get_segments(minishell->prompt, " ");
	minishell->lexer.option_buffer = get_segments(minishell->prompt, "-");
	minishell->lexer.ampersand_buffer = get_segments(minishell->prompt, "&");
	minishell->lexer.semicolon_buffer = get_segments(minishell->prompt, ";");
	put_lexer(minishell->lexer);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argv;
	(void)envp;
	if (argc != 1)
		return (0);
	while (1)
	{
		minishell.prompt = readline("minishell>");
		if (minishell.prompt)
		{
			add_history(minishell.prompt);
			lex_prompt(&minishell);
			free(minishell.prompt);
		}
		else
			return (ft_putstr_fd(2, "cant read input\n"), 1);
	}
	return (0);
}
