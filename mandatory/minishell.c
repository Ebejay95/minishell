/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 10:09:04 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

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
	{
		exit(0);
	}
}

static void	non_interactive_mode(t_minishell *minishell)
{
	char	*input;
	char	*temp;

	temp = NULL;
	input = NULL;
	temp = get_next_line(STDIN_FILENO);
	if (temp)
	{
		input = remove_chars(temp, "\n");
		free(temp);
	}
	minishell->prompt = input;
	return ;
}

int	is_only_whitespace(const char *str)
{
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

static int	handle_input(t_minishell *minishell)
{
	if (minishell->modus > 0)
		interactive_mode(&(*minishell));
	else if (minishell->modus == 0)
		non_interactive_mode(&(*minishell));
	if (minishell->prompt == NULL)
		return (1);
	minishell->tok_lst = NULL;
	minishell->exec_lst = NULL;
	minishell->pids = NULL;
	minishell->argc = 0;
	if (minishell->prompt)
	{
		minishell->last_exitcode = minishell->exitcode;
		minishell->leave = 0;
		lex_prompt(minishell);
		pre_exec_prep(minishell);
		pre_exec_checks(minishell);
		if (!minishell->leave)
			execute(minishell);
		if (minishell->modus && *minishell->prompt
			&& !is_only_whitespace(minishell->prompt))
			add_history(minishell->prompt);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	minishell.env_list = NULL;
	init_env_list(envp, &minishell);
	setup_signals(&minishell);
	initialize_minishell(&minishell, envp);
	if (argc >= 2)
	{
		if (access(argv[1], F_OK) == -1)
		{
			ft_printf("bash: %s: No such file or directory\n", argv[1]);
			minishell.last_exitcode = 127;
			minishell.leave = 1;
		}
	}
	while (1)
		if (handle_input(&minishell))
			break ;
	rl_clear_history();
	cleanup_minishell(&minishell, 0);
	return (minishell.exitcode);
}
