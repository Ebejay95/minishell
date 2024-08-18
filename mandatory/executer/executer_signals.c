/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:01:58 by chorst            #+#    #+#             */
/*   Updated: 2024/08/16 01:40:49 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	run_child_process(t_minishell *m, t_pipe_info *pi)
{
	signal(SIGINT, handle_child_process);
	signal(SIGQUIT, handle_child_process);
	if (pi->i > 0)
	{
		dup2(pi->prv_pipe, STDIN_FILENO);
		close(pi->prv_pipe);
	}
	if (m->cmd_seqs[pi->i + 1] != NULL)
	{
		close(pi->pipe_fd[0]);
		dup2(pi->pipe_fd[1], STDOUT_FILENO);
		close(pi->pipe_fd[1]);
	}
	run_seg(m, pi->i, STDIN_FILENO, STDOUT_FILENO);
	exit(0);
}

void	handle_parent_process(t_pipe_info *pi)
{
	if (pi->i > 0)
		close(pi->prv_pipe);
	if (pi->i < pi->total)
	{
		close(pi->pipe_fd[1]);
		pi->prv_pipe = pi->pipe_fd[0];
	}
}

void wait_for_children(t_minishell *m, t_pipe_info *pi)
{
    int j;
    int status;

    j = 0;
    while (j < pi->i)
    {
        waitpid(pi->pids[j], &status, 0);
        signal(SIGINT, handle_main_process);
        signal(SIGQUIT, handle_main_process);
        if (WIFEXITED(status))
            m->exitcode = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            m->exitcode = 128 + WTERMSIG(status);
        j++;
    }
}

void	setup_child_process(t_fd *fd)
{
	signal(SIGINT, handle_child_process);
	signal(SIGQUIT, handle_child_process);
	if (fd->last_input != fd->input)
	{
		dup2(fd->last_input, STDIN_FILENO);
		close(fd->last_input);
	}
	if (fd->last_output != fd->output)
	{
		dup2(fd->last_output, STDOUT_FILENO);
		close(fd->last_output);
	}
}

void run_parent_process(t_minishell *m, pid_t pid)
{
    int status;

    waitpid(pid, &status, 0);
    signal(SIGINT, handle_main_process);
    signal(SIGQUIT, handle_main_process);
    if (WIFEXITED(status))
        m->exitcode = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        m->exitcode = 128 + WTERMSIG(status);
}
