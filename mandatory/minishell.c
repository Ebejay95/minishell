/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/11 20:33:35 by jeberle          ###   ########.fr       */
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

// void	put_lexer(t_arglexer *lexer)
// {
// 	if (!lexer)
// 		return ;
// 	write (1, "is_space: ", 10);
// 	ft_putnbr(lexer->is_space);
// 	write (1, "\nis_dash: ", 10);
// 	ft_putnbr(lexer->is_dash);
// 	write (1, "\nis_bslash: ", 12);
// 	ft_putnbr(lexer->is_bslash);
// 	write (1, "\nop_quote: ", 11);
// 	ft_putnbr(lexer->op_quote);
// 	write (1, "\nop_dquote: ", 12);
// 	ft_putnbr(lexer->op_dquote);
// 	write (1, "\ncl_quote: ", 11);
// 	ft_putnbr(lexer->cl_quote);
// 	write (1, "\ncl_dquote: ", 12);
// 	ft_putnbr(lexer->cl_dquote);
// 	write(1, "\n---\n", 5);
// }
int	main (int argc, char **argv, char **envp)
{
	char	*pearl;
	(void) argv;
	(void) envp;
	if (argc != 1)
		return (0);
	pearl = readline("\x1b[34mminishell>\x1b[0m");
	if (pearl)
	{
		add_history(pearl);
		free(pearl);
	}
	else
	{
		ft_putstr_fd(2, "cant read input\n");
		return (1);
	}
	return (0);
}

//int	main (int argc, char **argv, char **envp)
//{
//	if (argc != 1)
//		return (0);
//
//	//char *exline;
//	//ft_printf("Hi Minishell!\n");
//	//ft_putallenv(envp);
//	//exline = ft_get_envline("PATH", envp);
//	// ft_printf(ft_color(exline, GREEN));
//
//	// t_arglexer **arglexers;
//	// int	i;
//	// (void) argv;
//	// (void) envp;
//
//	// if (argc < 2)
//	// 	return (0);
//	// i = 0;
//	// arglexers = malloc((argc - 1) * sizeof(t_arglexer *));
//	// if (!arglexers)
//	// 	return (1);
//	// i = 0;
//	// while (i < (argc - 1))
//	// {
//	// 	arglexers[i] = malloc((argc - 1) * sizeof(t_arglexer *));
//	// 	if (!arglexers[i])
//	// 		return (1);
//	// 	arglexers[i]->is_space = 0;
//	// 	arglexers[i]->is_dash = 0;
//	// 	arglexers[i]->is_bslash = 0;
//	// 	arglexers[i]->op_quote = 0;
//	// 	arglexers[i]->op_dquote = 0;
//	// 	arglexers[i]->cl_quote = 0;
//	// 	arglexers[i]->cl_dquote = 0;
//	// 	i++;
//	// }
//	// i = 0;
//	// while (i < (argc - 1))
//	// {
//	// 	put_lexer(arglexers[i]);
//	// 	i++;
//	// }
//	// i = 0;
//	// while (i < (argc - 1))
//	// {
//	// 	free(arglexers[i]);
//	// 	i++;
//	// }
//	// free(arglexers);
//	return (0);
//}