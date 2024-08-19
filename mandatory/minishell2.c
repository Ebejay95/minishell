/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:04:21 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 09:19:57 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Cleanup the minishell struct
void	free_token_list(t_list *list)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;

	current = list;
	while (current)
	{
		next = current->next;
		token = current->content;
		if (token != NULL)
			newfree_token(token);
		free(current);
		current = next;
	}
}

void	clear_cmd_seqs(t_minishell *minishell)
{
	int	i;

	i = 0;
	while (i < MAXPIPS)
	{
		if (minishell->cmd_seqs[i])
		{
			free_token_list(minishell->cmd_seqs[i]);
			minishell->cmd_seqs[i] = NULL;
		}
		i++;
	}
}

void	clear_exec_seqs(t_minishell *minishell)
{
	int	i;

	i = 0;
	while (i < MAXPIPS)
	{
		if (minishell->exec_seqs[i])
		{
			free_token_list(minishell->exec_seqs[i]);
			minishell->exec_seqs[i] = NULL;
		}
		i++;
	}
}

void	cleanup_minishell(t_minishell *minishell)
{
	reset_minishell_args(minishell);
	mlstclear(minishell->exec_lst);
	mlstclear(minishell->tok_lst);
	ft_envlstclear(minishell);
	if (minishell->pids)
		free(minishell->pids);
	clear_cmd_seqs(minishell);
	clear_exec_seqs(minishell);
}

void	initialize_minishell(t_minishell *minishell, char **envp)
{
	minishell->leave = 0;
	minishell->last_exitcode = 0;
	minishell->exitcode = 0;
	minishell->envp = envp;
	minishell->prompt = NULL;
}
