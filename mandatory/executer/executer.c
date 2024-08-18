/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/16 01:40:08 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

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
// Neue Hilfsfunktion
int has_pipes(t_list *tok_lst)
{
    t_list *current = tok_lst;
    while (current)
    {
        t_token *token = (t_token *)current->content;
        if (token->token == PIPE)
            return 1;
        current = current->next;
    }
    return 0;
}


void execute(t_minishell *m)
{
    m->last_exitcode = m->exitcode;
    m->exitcode = 0;

    m->pipes = 0;
    if (has_pipes(m->tok_lst))
    {
    	split_pipes(m);
        execute_with_pipes(m);
        reset_minishell_args(m);
    	reset_sequences(m);
    }
    else
    {
        prexecute(m, 0);
        if (g_global == 0)
		{
            run_seg(m, 0, STDIN_FILENO, STDOUT_FILENO);
		}
        reset_minishell_args(m);
    	if (m->tok_lst)
    	{
    	    mlstclear(m->tok_lst);
    	    m->tok_lst = NULL;
    	}
    	if (m->exec_lst)
    	{
    	    mlstclear(m->exec_lst);
    	    m->exec_lst = NULL;
    	}
    }
}
