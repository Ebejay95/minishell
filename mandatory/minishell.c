/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/11 22:13:44 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

char	*ft_get_envline(char *needle, char **envp)
{
	int		envdx;
	char	*pthl;
	char	*pthl_start;

	envdx = 0;
	while (envp[envdx] != NULL)
	{
		pthl_start = ft_strstr(envp[envdx], needle);
		if (pthl_start == envp[envdx])
		{
			pthl = ft_calloc((ft_strlen(envp[envdx]) + 1), sizeof(char));
			if (pthl == NULL)
				return (NULL);
			ft_strcpy(pthl, envp[envdx]);
		}
		envdx++;
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
// 			break;
// 	}
// 	return (0);
// }

void	ft_putallenv(char **env)
{
	int	envdx;
	char	*print;

	envdx = 0;
	while (env[envdx] != 0)
	{
		print = ft_color(env[envdx], YELLOW);
		ft_printf(print);
		ft_printf("\n");
		free(print);
		envdx++;
	}
}

void	put_lexer(t_arglexer lexer)
{
	write (1, "is_space: ", 10);
	ft_putnbr(lexer.is_space);
	write (1, "\nis_dash: ", 10);
	ft_putnbr(lexer.is_dash);
	write (1, "\nis_bslash: ", 12);
	ft_putnbr(lexer.is_bslash);
	write (1, "\nop_quote: ", 11);
	ft_putnbr(lexer.op_quote);
	write (1, "\nop_dquote: ", 12);
	ft_putnbr(lexer.op_dquote);
	write (1, "\ncl_quote: ", 11);
	ft_putnbr(lexer.cl_quote);
	write (1, "\ncl_dquote: ", 12);
	ft_putnbr(lexer.cl_dquote);
	write(1, "\n---\n", 5);
}


void	lex_pearl(char *pearl)
{
	t_arglexer arglexer;

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



int	main (int argc, char **argv, char **envp)
{
	char	*pearl;
	char	*exline;
	(void) argv;
	(void) envp;
	//ft_putallenv(envp);
	if (argc != 1)
		return (0);
	pearl = readline("\x1b[34mminishell>\x1b[0m");
	if (pearl)
	{
		add_history(pearl);
		lex_pearl(pearl);
		free(pearl);
		exline = ft_get_envline("PATH", envp);
		ft_printf(ft_color(exline, GREEN));
	}
	else
	{
		ft_putstr_fd(2, "cant read input\n");
		return (1);
	}
	return (0);
}
