/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/07 08:46:42 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

// |---------------------------------------------------------------------------|

void	execute(t_minishell *m)
{
	int		pipe_fd[2];
	int		prev_pipe_read;
	pid_t	pid;
	int		status;
	int		i;
	t_list	*seq;

	ft_printf(G"execute\n"D);
	prev_pipe_read = -1;
	i = 0;
	m->pipes = 0;
	pre_exec_prep(m);
	pre_exec_checks(m);
	ft_printf("leave %i\n", m->leave);
	if (m->leave)
		return ;
	if (DEBUG == 1)
	{
		ft_printf(Y"EXECUTE:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	m->cmd_sequences = split_by_pipe(m);
	if (m->pipes != 0)
	{
		if (DEBUG == 1)
			ft_printf(Y"PIPE SEGMENTS:\n"D);
		while (m->cmd_sequences[i] != NULL)
		{
			if (DEBUG == 1)
			{
				ft_printf(Y"seg %i:\n"D, i);
				ft_lstput(&(m->cmd_sequences[i]), put_token, '\n');
			}
			seq = m->cmd_sequences[i];
			if (m->cmd_sequences[i + 1] != NULL)
			{
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe failed");
					exit(EXIT_FAILURE);
				}
			}
			pid = fork();
			if (pid == -1)
			{
				perror("fork failed");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0)
			{
				if (prev_pipe_read != -1)
				{
					if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
					{
						perror("dup2 failed for stdin");
						exit(EXIT_FAILURE);
					}
					close(prev_pipe_read);
				}
				if (m->cmd_sequences[i + 1] != NULL)
				{
					close(pipe_fd[0]);
					if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
					{
						perror("dup2 failed for stdout");
						exit(EXIT_FAILURE);
					}
					close(pipe_fd[1]);
				}
				if (handle_redirections(m, m->cmd_sequences[i], seq) == -1)
					exit(EXIT_FAILURE);
				run_command(m, seq);
				exit(m->exitcode);
			}
			else
			{
				if (prev_pipe_read != -1)
					close(prev_pipe_read);
				if (m->cmd_sequences[i + 1] != NULL)
				{
					close(pipe_fd[1]);
					prev_pipe_read = pipe_fd[0];
				}
			}
			i++;
		}
	}
	else
	{
		if (handle_redirections(m, m->tok_lst, m->tok_lst) == -1)
			return ;
		run_command(m, m->tok_lst);
	}
	// while (wait(&status) > 0)
	// 	ft_printf("waiting ... \n");
	if (WIFEXITED(status))
		m->exitcode = WEXITSTATUS(status);
}
