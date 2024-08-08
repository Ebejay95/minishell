/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:17:22 by chorst            #+#    #+#             */
/*   Updated: 2024/08/08 17:03:03 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function the main process signal
void	handle_main_process(int sig)
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
		write(STDOUT_FILENO, "Quit: 3\n", 8);
	}
}

// Function that handles every child process signal
void	handle_child_process(int sig)
{
	int	status;

	status = isatty(STDIN_FILENO);
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "", 1);
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit: 3\n", 8);
	}
}

// sleep(1);
// printf("...3\n");
// sleep(1);
// printf("...2\n");
// sleep(1);
// printf("...1\n");
// sleep(1);
// mode 0 = interactive mode
// mode > 0 = non-interactive mode
// Function that sets up the signals
void	setup_signals(t_minishell *minishell)
{
	struct sigaction	sa;
	struct termios		tty;

	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	minishell->modus = isatty(STDIN_FILENO);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (minishell->modus)
	{
		sa.sa_handler = handle_main_process;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
	{
		sa.sa_handler = handle_child_process;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
}
