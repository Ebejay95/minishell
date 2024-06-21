/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/20 12:26:51 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that parses the input into a table using ft_split ### TEMPORARY EXAMPLE ###
char	**parse_input(char *prompt, int *argc)
{
	char	**argv;
	int		i;
	char	*clean_prompt;

	clean_prompt = input_cleaner(prompt);
	argv = ft_split(clean_prompt, ' ');
	free(clean_prompt);
	i = 0;
	while (argv[i])
		i++;
	*argc = i;
	return (argv);
}

// Function that choses which bultin to execute based on the prompt
void	execute_command(char *prompt, char ***envp)
{
	int		argc;
	char	**argv;

	argv = parse_input(prompt, &argc);
	// if (ft_strcmp(prompt, "cd") == 0)
	// 	ft_cd(*envp, argv[1]);
	// if (ft_strcmp(prompt, "echo") == 0)
	// 	ft_echo(argv);
	if (ft_strcmp(prompt, "env") == 0)
		ft_env(envp);
	if (ft_strcmp(prompt, "exit") == 0)
		exit(0);
	if (ft_strcmp(prompt, "export") == 0)
		ft_export(argc, argv, &envp);
	if (ft_strcmp(prompt, "pwd") == 0)
		ft_pwd(argv);
	// if (ft_strcmp(prompt, "unset") == 0)		unset
	// 	ft_unset(envp, argv[1]);
	// lex_prompt(prompt);
}

// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argv;
	(void)envp;
	if (argc != 1)
		return (0);
	minishell.envp = envp;
	while (1)
	{
		minishell.prompt = readline("\033[0;31m8==D \033[0m");
		minishell.prompt = input_cleaner(minishell.prompt);
		minishell.ast = ft_btreenew(NULL);
		if (minishell.prompt)
		{
			execute_command(minishell.prompt, &minishell.envp);
			add_history(minishell.prompt);
			lex_prompt(&minishell);
			parse(&minishell);
			free(minishell.prompt);
		}
	}
	return (0);
}
