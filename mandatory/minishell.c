/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/23 14:49:31 by chorst           ###   ########.fr       */
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
		if (!(ft_strcmp(argv[0], "exit")))
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

// Function that handles the input from the user or the script
int handle_input(t_minishell *minishell, t_envlst **envlst)
{
	if (minishell->is_interactive)
		interactive_mode(&(*minishell));
	else
		non_interactive_mode(&(*minishell));
	if (!minishell->prompt)
		return (0);
	minishell->tok_lst = NULL;
	minishell->ast = ft_btreenew(NULL);
	if (minishell->prompt[0] != '\0')
	{
		execute_command(minishell->prompt, envlst);
		if (minishell->is_interactive)
		{
			add_history(minishell->prompt);
		}
		// lex_prompt(minishell);
		// parse(minishell);
	}
	cleanup_minishell(minishell);
	return (1);
}

// (was vorher in der main war, steht jetzt in handle_input (einfach ausgelagert))
// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	t_envlst	*envlst;

	envlst = init_env_list(envp);
	(void)argv;
	if (argc != 1)
		return (0);
	setup_signals(&minishell);
	initialize_minishell(&minishell, envp);
	while (1)
	{
		if (!handle_input(&minishell, &envlst))
			break ;
	}
	rl_clear_history();
	return (0);
}


// Notes for push:
// added new handle_input function to outsource the main function, therfore the makefile had to be changed to include the new file
// made changes in the signal.c file to make it work with the new structure and to handle also non-interactive mode
// added a new file called main_helper.c to include helper functions for the main function
// added interactive_mode and non_interactive_mode functions to handle the different modes
