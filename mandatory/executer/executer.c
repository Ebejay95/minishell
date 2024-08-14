/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 12:46:46 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	execute(t_minishell *m)
{
	int		status;
	int		i;
	int		pipe_fd[2];
	int		prev_pipe_read;
	pid_t	*pids;
	int		j;

	if (DEBUG == 1)
	{
		ft_printf(Y"execute:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	prev_pipe_read = STDIN_FILENO;
	i = 0;
	j = 0;
	m->pipes = 0;
	split_pipes(m, &(m->cmd_seqs), &(m->exec_seqs));
	if (m->pipes != 0)
	{
		if (DEBUG == 1)
			ft_printf(G"PIPE SEGMENTS:\n"D);
		pids = malloc(sizeof(pid_t) * (m->pipes + 1));
		if (!pids)
		{
			ft_fprintf(2, "Memory allocation failed\n");
			return ;
		}
		while (m->cmd_seqs[i] != NULL)
		{
			if (DEBUG == 1)
			{
				ft_printf(Y"PIPE:\n"D);
				ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
			}
			prexecute(m, &(m->cmd_seqs[i]), &(m->exec_seqs[i]));
			if (DEBUG == 1)
			{
				ft_printf(Y"cmd_seqs:\n"D);
				ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
				ft_printf(Y"exec_seqs:\n"D);
				ft_lstput(&(m->exec_seqs[i]), put_token, '\n');
			}
			if (m->cmd_seqs[i + 1] != NULL)
			{
				if (pipe(pipe_fd) == -1)
				{
					ft_fprintf(2, "Pipe creation failed\n");
					free(pids);
					return ;
				}
			}
			else
			{
				pipe_fd[1] = STDOUT_FILENO;
			}
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			pids[i] = fork();
			if (pids[i] == 0)
			{
				signal(SIGINT, handle_child_process);
				signal(SIGQUIT, handle_child_process);
				if (i > 0)
				{
					dup2(prev_pipe_read, STDIN_FILENO);
					close(prev_pipe_read);
				}
				if (m->cmd_seqs[i + 1] != NULL)
				{
					close(pipe_fd[0]);
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
				}
				run_seg(m, m->exec_seqs[i], STDIN_FILENO, STDOUT_FILENO);
				exit(0);
			}
			else if (pids[i] < 0)
			{
				ft_fprintf(2, "Fork failed\n");
				free(pids);
				return ;
			}
			if (i > 0)
				close(prev_pipe_read);
			if (m->cmd_seqs[i + 1] != NULL)
			{
				close(pipe_fd[1]);
				prev_pipe_read = pipe_fd[0];
			}
			i++;
		}
		while (j < i)
		{
			waitpid(pids[j], &status, 0);
			signal(SIGINT, handle_main_process);
			signal(SIGQUIT, handle_main_process);
			if (WIFEXITED(status))
				m->exitcode = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				m->exitcode = 128 + WTERMSIG(status);
			j++;
		}
		free(pids);
	}
	else
	{
		if (DEBUG == 1)
			ft_printf(G"EXEC without pipe\n"D);
		prexecute(m, &(m->tok_lst), &(m->exec_lst));
		run_seg(m, m->tok_lst, STDIN_FILENO, STDOUT_FILENO);
	}
	reset_sequences(m);
}
