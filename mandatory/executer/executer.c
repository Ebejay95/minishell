/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 13:36:36 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	debug_print(t_minishell *m, int i)
{
	if (DEBUG == 1)
	{
		ft_printf(Y"execute:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
		if (m->pipes != 0)
			ft_printf(G"PIPE SEGMENTS:\n"D);
		if (m->pipes != 0 && m->cmd_seqs[i] != NULL)
		{
			ft_printf(Y"PIPE:\n"D);
			ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
		}
		if (m->pipes == 0)
			ft_printf(G"EXEC without pipe\n"D);
		ft_printf(Y"cmd_seqs:\n"D);
		ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
		ft_printf(Y"exec_seqs:\n"D);
		ft_lstput(&(m->exec_seqs[i]), put_token, '\n');
	}
}

void	setup_pipes(t_pipe_info *pi)
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
	run_seg(m, m->exec_seqs[pi->i], STDIN_FILENO, STDOUT_FILENO);
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

void	wait_for_children(t_minishell *m, t_pipe_info *pi)
{
	int	j;
	int	status;

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

int	allocate_pids(t_pipe_info *pi, int pipes)
{
	pi->pids = malloc(sizeof(pid_t) * (pipes + 1));
	if (!pi->pids)
	{
		ft_fprintf(2, "Memory allocation failed\n");
		return (0);
	}
	return (1);
}

void	exec_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	fork_and_execute(t_minishell *m, t_pipe_info *pi)
{
	pi->pids[pi->i] = fork();
	if (pi->pids[pi->i] == 0)
	{
		run_child_process(m, pi);
		exit(1);
	}
	else if (pi->pids[pi->i] < 0)
	{
		ft_fprintf(2, "Fork failed\n");
		return (0);
	}
	return (1);
}

int	process_pipecommand(t_minishell *m, t_pipe_info *pi)
{
	debug_print(m, pi->i);
	prexecute(m, &(m->cmd_seqs[pi->i]), &(m->exec_seqs[pi->i]));
	setup_pipes(pi);
	exec_signals();
	if (!fork_and_execute(m, pi))
		return (0);
	handle_parent_process(pi);
	return (1);
}

void	execute_with_pipes(t_minishell *m)
{
	t_pipe_info	pi;

	pi.prv_pipe = STDIN_FILENO;
	pi.i = 0;
	pi.total = m->pipes;
	if (!allocate_pids(&pi, m->pipes))
		return ;
	while (m->cmd_seqs[pi.i] != NULL)
	{
		if (!process_pipecommand(m, &pi))
		{
			free(pi.pids);
			return ;
		}
		pi.i++;
	}
	wait_for_children(m, &pi);
	free(pi.pids);
}

void	execute(t_minishell *m)
{
	m->pipes = 0;
	split_pipes(m, &(m->cmd_seqs), &(m->exec_seqs));
	if (m->pipes != 0)
	{
		execute_with_pipes(m);
	}
	else
	{
		debug_print(m, 0);
		prexecute(m, &(m->tok_lst), &(m->exec_lst));
		run_seg(m, m->tok_lst, STDIN_FILENO, STDOUT_FILENO);
	}
	reset_sequences(m);
}
