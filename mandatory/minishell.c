/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/12 19:12:39 by chorst           ###   ########.fr       */
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

void	put_lexer(t_arglexer lexer)
{
	printf("is_space: %d\n", lexer.is_space);
	printf("is_dash: %d\n", lexer.is_dash);
	printf("is_bslash: %d\n", lexer.is_bslash);
	printf("op_quote: %d\n", lexer.op_quote);
	printf("op_dquote: %d\n", lexer.op_dquote);
	printf("cl_quote: %d\n", lexer.cl_quote);
	printf("cl_dquote: %d\n", lexer.cl_dquote);
	printf("---\n");
}

void	lex_prompt(char *prompt)
{
	t_arglexer	arglexer;

	ft_printf("\x1b[36m%s\x1b[0m\n", prompt);
	arglexer.position = 0;
	arglexer.is_space = 0;
	arglexer.is_dash = 0;
	arglexer.is_bslash = 0;
	arglexer.op_quote = 0;
	arglexer.op_dquote = 0;
	arglexer.cl_quote = 0;
	arglexer.cl_dquote = 0;
	arglexer.is_option = 0;
	arglexer.options = NULL;
	put_lexer(arglexer);
}

int	main(int argc, char **argv, char **envp)
{
	char	*prompt;
	char	*exline;

	(void)argv;
	(void)envp;
	// ft_putallenv(envp);
	if (argc != 1)
		return (0);
	prompt = readline(R"m"Y"i"G"n"B"i"C"s"M"h"R"e"Y"l"G"l"B" ""%"D" ");
	if (prompt)
	{
		add_history(prompt);
		lex_prompt(prompt);
		free(prompt);
		exline = getenv("PATH");
		ft_printf(ft_color(exline, GREEN));
		// or instead
		// ft_printf("%s %s %s\n", G, exline, D);
	}
	else
	{
		ft_putstr_fd(2, "cant read input\n");
		return (1);
	}
	return (0);
}
