/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 21:00:44 by jeberle          ###   ########.fr       */
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

int	fork_and_execute(t_minishell *m, t_pipe_info *pi)
{
	pi->pids[pi->i] = fork();
	if (pi->pids[pi->i] == 0)
	{
		run_child_process(m, pi);
		write(1, "fork\n", 5);
		exit(1);
	}
	else if (pi->pids[pi->i] < 0)
	{
		ft_fprintf(2, "Fork failed\n");
		return (0);
	}
	return (1);
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
		if (g_global == 0)
			run_seg(m, m->tok_lst, STDIN_FILENO, STDOUT_FILENO);
	}
	reset_sequences(m);
}
