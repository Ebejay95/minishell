/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/03 14:23:28 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that defines the interactive mode
static void	interactive_mode(t_minishell *minishell)
{
	minishell->prompt = readline("🍕🚀🌈🦄🍺 ");
	if (!minishell->prompt)
		return ((void)printf("\033[1A🍕🚀🌈🦄🍺 exit\n"));
}

// Function that defines the non-interactive mode
// Hier wird read anstatt readline verwendet
static void	non_interactive_mode(t_minishell *minishell)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, 1024);
	if (bytes_read <= 0)
		return ;
	buffer[bytes_read] = '\0';
	minishell->prompt = strdup(buffer);
	if (!minishell->prompt)
		return (perror("strdup"));
}

// Function that handles the input from the user or the script
static int	handle_input(t_minishell *minishell)
{
	if (minishell->is_interactive)
		interactive_mode(&(*minishell));
	else
		non_interactive_mode(&(*minishell));
	if (!minishell->prompt)
		return (0);
	minishell->tok_lst = NULL;
	minishell->ast = ft_btreenew(NULL);
	if (minishell->prompt)
	{
		lex_prompt(minishell);
		execute(minishell);
		// parse(minishell);
		if (minishell->is_interactive)
			add_history(minishell->prompt);
	}
	return (1);
}

// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	minishell.exitcode = 0;
	minishell.env_list = NULL;
	init_env_list(envp, &minishell);
	(void)argv;
	if (argc != 1)
		return (0);
	setup_signals(&minishell);
	initialize_minishell(&minishell, envp);
	while (1)
		if (!handle_input(&minishell))
			break ;
	cleanup_minishell(&minishell);
	rl_clear_history();
	return (0);
}
