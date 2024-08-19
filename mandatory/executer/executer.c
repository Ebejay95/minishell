/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 07:46:59 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	has_pipes(t_list *tok_lst)
{
	t_list	*current;
	t_token	*token;

	current = tok_lst;
	while (current)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

void	initialize_arrays(t_minishell *m)
{
	int	i;

	i = 0;
	while (i < MAXPIPS)
	{
		m->cmd_seqs[i] = NULL;
		m->exec_seqs[i] = NULL;
		m->args[i] = NULL;
		i++;
	}
}

void	cleanup_lists(t_minishell *m)
{
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

void	execute(t_minishell *m)
{
	initialize_arrays(m);
	m->last_exitcode = m->exitcode;
	m->exitcode = 0;
	m->pipes = 0;
	if (has_pipes(m->tok_lst))
	{
		split_pipes(m);
		execute_with_pipes(m);
		reset_minishell_args(m);
		cleanup_minishell(m);
	}
	else
	{
		prexecute(m, 0);
		if (g_global == 0)
			run_seg(m, 0, STDIN_FILENO, STDOUT_FILENO);
		reset_minishell_args(m);
		cleanup_lists(m);
	}
}
