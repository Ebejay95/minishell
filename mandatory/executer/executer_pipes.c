/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:00:34 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 21:00:44 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static void	setup_pipes(t_pipe_info *pi)
{
	if (pi->i < pi->total)
	{
		if (pipe(pi->pipe_fd) == -1)
		{
			ft_fprintf(2, "Pipe creation failed\n");
			return ;
		}
	}
	else
	{
		pi->pipe_fd[1] = STDOUT_FILENO;
	}
}

static int	process_pipecommand(t_minishell *m, t_pipe_info *pi)
{
	prexecute(m, pi->i);
	if (g_global == 0)
	{
		setup_pipes(pi);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (!fork_and_execute(m, pi))
		{
			return (0);
		}
		handle_parent_process(pi);
	}
	return (1);
}

void	execute_with_pipes(t_minishell *m)
{
	t_pipe_info	pi;

	pi.prv_pipe = STDIN_FILENO;
	pi.i = 0;
	pi.total = m->pipes;
	if (!allocate_pids(m, m->pipes))
		return ;
	while (m->cmd_seqs[pi.i] != NULL)
	{
		if (!process_pipecommand(m, &pi))
		{
			free(m->pids);
			m->pids = NULL;
			return ;
		}
		reset_minishell_args(m);
		pi.i++;
	}
	wait_for_children(m, &pi);
	free(m->pids);
	m->pids = NULL;
}
