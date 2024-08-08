/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/07 14:56:27 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that defines the interactive mode
static void	interactive_mode(t_minishell *minishell)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd == -1)
	{
		perror("Failed to open /dev/tty");
		exit(1);
	}
	if (dup2(tty_fd, STDIN_FILENO) == -1)
	{
		perror("Failed to redirect stdin");
		exit(1);
	}
	close(tty_fd);
	if (minishell->modus == 1)
		minishell->prompt = readline("🍕🚀🌈🦄🍺 ");
	if (!minishell->prompt)
		exit(0);
}

// Function that defines the non-interactive mode
// Hier wird read anstatt readline verwendet
static void	non_interactive_mode(t_minishell *minishell)
{
	char	*input;
	char	*temp;

	temp = NULL;
	input = NULL;
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
	if (minishell->modus > 0)
		interactive_mode(&(*minishell));
	else if (minishell->modus == 0)
		non_interactive_mode(&(*minishell));
	if (minishell->prompt == NULL)
		return (1);
	minishell->tok_lst = NULL;
	minishell->ast = ft_btreenew(NULL);
	if (minishell->prompt)
	{
		minishell->leave = 0;
		lex_prompt(minishell);
		execute(minishell);
		if (minishell->modus)
			add_history(minishell->prompt);
	}
	return (0);
}

// if (argc >= 2) set non inteactive mode (we think)
// ft_printf("verarbeite %s\n", argv[1]);
// open file argv[1]
// get file content
// use as prompt
// if argv[1] == irgendweinscheisse
// set exitcode 127, leave by no scuh file or directorz
// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	init_env_list(envp, &minishell);
	(void)argc;
	(void)argv;
	setup_signals(&minishell);
	initialize_minishell(&minishell, envp);
	while (1)
		if (handle_input(&minishell))
			break ;
	rl_clear_history();
	cleanup_minishell(&minishell);
	return (minishell.exitcode);
}
