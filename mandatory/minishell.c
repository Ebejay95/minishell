/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/05 14:14:33 by jkauker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that parses the input into a table using ft_split
char	**parse_input(char *prompt, int *argc)
{
	char	**argv;
	int		i;

	argv = ft_split(prompt, ' ');
	i = 0;
	while (argv[i])
		i++;
	*argc = i;
	return (argv);
}

// Function that choses which bultin to execute based on the prompt
void	execute_command(char *prompt, t_envlst **envlst)
{
	int		argc;
	char	**argv;

	argv = parse_input(prompt, &argc);
	if (argv[0])
	{
		// if (ft_strcmp(prompt, "cd") == 0)		// ft_cd
		// 	ft_cd(*envp, argv[1]);
		// if (ft_strcmp(prompt, "echo") == 0)		// ft_echo
		// 	ft_echo(argv);
		if (ft_strcmp(prompt, "env") == 0)
			ft_env(*envlst);
		if (ft_strcmp(prompt, "exit") == 0)
			exit(0);
		if (!(ft_strcmp(argv[0], "export")) || !(is_var_name(*envlst, argv)))
			ft_export(&envlst, argc, argv);
		if (ft_strcmp(prompt, "pwd") == 0)
			ft_pwd(argv);
		if (ft_strcmp(argv[0], "unset") == 0)
			ft_unset(envlst, argv);
	}
	// lex_prompt(prompt);
	while (argc >= 0)
	{
		free(argv[argc]);
		argc--;
	}
}

// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	t_envlst	*envlst;

	envlst = init_env_list(envp);
	(void)argv;
	(void)envp;
	if (argc != 1)
		return (0);
	setup_signals();
	minishell.envp = envp;
	while (1)
	{
		minishell.envp = envp;
		minishell.prompt = readline("\033[0;31m  ════ ⋆★⋆ ════ \033[0m");
		minishell.prompt = input_cleaner(minishell.prompt);
		// minishell.ast = ft_btreenew(NULL);
		// t_token		*expipeone;
		// expipeone = create_token(PIPE);
		// ast_add(&minishell.ast, minishell.ast, "next", expipeone);
		// ft_btreeput(&minishell.ast, put_token);
		if (minishell.prompt)
		{
			execute_command(minishell.prompt, &envlst);
			add_history(minishell.prompt);
			// lex_prompt(&minishell);
			// parse(&minishell);
			free(minishell.prompt);
		}
	}
	rl_clear_history();
	return (0);
}
