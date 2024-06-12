/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/12 18:40:55 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Used instead of this function: getenv
char	*ft_get_envline(char *needle, char **envp)
{
	int		i;
	char	*pthl;
	char	*pthl_start;

	i = 0;
	while (envp[i] != NULL)
	{
		pthl_start = ft_strstr(envp[i], needle);
		if (pthl_start == envp[i])
		{
			pthl = ft_calloc((ft_strlen(envp[i]) + 1), sizeof(char));
			if (pthl == NULL)
				return (NULL);
			ft_strcpy(pthl, envp[i]);
		}
		i++;
	}
	while (*pthl != '=' && *pthl != '\0')
	{
		pthl++;
	}
	if (*pthl == '=')
		pthl++;
	return (pthl);
}

// char	*ft_get_envline(char *needle, char **envp)
// {
// 	char *c;

// 	while (1)
// 	{
// 		c = get_next_line(1);
// 		if (ft_strncmp(c, "exit", 4) == 0)
// 			break ;
// 	}
// 	return (0);
// }

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

void	lex_pearl(char *pearl)
{
	t_arglexer	arglexer;

	ft_printf("\x1b[36m%s\x1b[0m\n", pearl);
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
	char	*pearl;
	char	*exline;

	(void)argv;
	(void)envp;
	// ft_putallenv(envp);
	if (argc != 1)
		return (0);
	pearl = readline(R"m"Y"i"G"n"B"i"C"s"M"h"R"e"Y"l"G"l"B" ""%"D" ");
	if (pearl)
	{
		add_history(pearl);
		lex_pearl(pearl);
		free(pearl);
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
