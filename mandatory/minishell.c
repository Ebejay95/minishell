/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/06 21:06:38 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that defines the interactive mode
static void	interactive_mode(t_minishell *minishell)
{
	printf("readline waits for input\n");
	printf("minishell->prompt3: %s\n", minishell->prompt);
	minishell->prompt = readline("🍕🚀🌈🦄🍺 ");
	printf("\nThats the prompt 1: %s\n", minishell->prompt);
	if (!minishell->prompt)
	{
		printf("Thats the prompt 2: %s\n", minishell->prompt);
		printf("exit\n");
		exit(0);
	}
	return ;
}

// Function that defines the non-interactive mode
// Hier wird read anstatt readline verwendet
static void	non_interactive_mode(t_minishell *minishell)
{
	char	*input;
	char	*temp;

	temp = NULL;
	input = NULL;
	printf("Im inside non_interactive mode\n");
	temp = get_next_line(STDIN_FILENO);
	if (temp)
		input = remove_chars(temp, "\n");
	free(temp);
	minishell->prompt = input;
	return ;
}

// Function that handles the input from the user or the script
static int	handle_input(t_minishell *minishell)
{
	printf("Modus call 3: %d\n", minishell->modus);
	printf("minishell->prompt2: %s\n", minishell->prompt);
	if (minishell->modus > 0)
		interactive_mode(&(*minishell));
	else if (minishell->modus == 0)
		non_interactive_mode(&(*minishell));
	if (minishell->prompt == NULL)
	{
		printf("Im the killer\n");
		return (1);
	}
	minishell->tok_lst = NULL;
	minishell->ast = ft_btreenew(NULL);
	if (minishell->prompt)
	{
		lex_prompt(minishell);
		execute(minishell);
		// parse(minishell);
		if (minishell->modus)
			add_history(minishell->prompt);
		printf("minishell->prompt1: %s\n", minishell->prompt);
		// minishell->modus = isatty(STDIN_FILENO);
	}
	return (0);
}

// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	printf(G"PROGRAMM START\n"D);
	t_minishell	minishell;

	minishell.exitcode = 0;
	minishell.env_list = NULL;
	init_env_list(envp, &minishell);
	(void)argv;
	if (argc != 1)
		return (0);
	setup_signals(&minishell);
	printf("Modus call 1: %d\n", minishell.modus);
	initialize_minishell(&minishell, envp);
	while (1)
		if (handle_input(&minishell))
			break ;
	rl_clear_history();
	cleanup_minishell(&minishell);
	printf("Modus call 2: %d\n", minishell.modus);
	return ((void)printf(R"END OF PROGRAMM (0)\n"D), 0);
}
