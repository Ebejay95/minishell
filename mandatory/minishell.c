/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/16 20:14:18 by jeberle          ###   ########.fr       */
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
		if (ft_strcmp(argv[0], "cd") == 0)
			ft_cd(argc, argv, &envlst);
		if (ft_strcmp(argv[0], "echo") == 0)
			ft_echo(argv);
		if (ft_strcmp(prompt, "env") == 0)
			ft_env(*envlst);
		if (ft_strcmp(prompt, "exit") == 0)
			ft_exit(argv);
		if (!(ft_strcmp(argv[0], "export")) || !(is_var_name(*envlst, argv)))
			ft_export(argc, argv, &envlst);
		if (ft_strcmp(prompt, "pwd") == 0)
			ft_pwd(argv);
		if (ft_strcmp(argv[0], "unset") == 0)
			ft_unset(envlst, argv);
	}
	while (argc >= 0)
		free(argv[argc--]);
}

// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;	t_envlst	*envlst;

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
		minishell.prompt = readline("🚀 ");
		if (!minishell.prompt)
		{
			printf("\033[1A🚀 exit\n");
			exit(0);
		}
		minishell.tok_lst = NULL;
		minishell.ast = ft_btreenew(NULL);
		if (minishell.prompt)
		{
			execute_command(minishell.prompt, &envlst);
			add_history(minishell.prompt);
			// lex_prompt(&minishell);
			parse(&minishell);
			free(minishell.prompt);
		}
	}
	rl_clear_history();
	return (0);
}

// settings.json

// {
// 	"files.associations": {
// 		"pthread.h": "c",
// 		"minishell.h": "c",
// 		"history.h": "c",
// 		"limits.h": "c"
// 	}
// }
