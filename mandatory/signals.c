/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:17:22 by chorst            #+#    #+#             */
/*   Updated: 2024/08/06 20:35:41 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that handles the signals
void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_non_interacive_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit: 3\n", 8);
	}
}


// mode 0 = interactive mode
// mode > 0 = non-interactive mode
// Function that sets up the signals
void	setup_signals(t_minishell *minishell)
{
	struct sigaction	sa;

	minishell->modus = isatty(STDIN_FILENO);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;


	if (minishell->modus > 0)
	{
		sa.sa_handler = handle_signal;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
		printf(G"Interactive mode is running...\n"D);
	}
	else if (minishell->modus == 0)
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
		ft_printf(Y"Non-interactive mode is running...\n"D);
		sleep(1);
		printf("...3\n");
		sleep(1);
		printf("...2\n");
		sleep(1);
		printf("...1\n");
		sleep(1);
	}
}
