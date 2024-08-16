/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:17:22 by chorst            #+#    #+#             */
/*   Updated: 2024/08/16 01:01:30 by jeberle          ###   ########.fr       */
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
		signal(SIGQUIT, SIG_IGN);
}

// Function that handles every child process signal
void	handle_child_process(int sig)
{
	if (sig == SIGINT)
		write(STDOUT_FILENO, "", 1);
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit: 3\n", 8);
		exit(131);
	}
}

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

void	reset_signals(void)
{
	struct termios	tty;

	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
}
