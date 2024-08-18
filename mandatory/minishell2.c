/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:04:21 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 21:00:44 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_envlstclear(t_minishell *minishell)
{
	t_envlst	*current;
	t_envlst	*next;

	if (minishell == NULL || minishell->env_list == NULL)
		return ;
	current = minishell->env_list;
	while (current != NULL)
	{
		next = current->next;
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	minishell->env_list = NULL;
}

// Cleanup the minishell struct
void free_token_list(t_list *list)
{
    t_list *current = list;
    while (current)
    {
        t_list *next = current->next;
        t_token *token = current->content;

        if (token != NULL) {
            newfree_token(token);  // Token vollständig freigeben
        }

        free(current);  // Freigeben der Listenknoten
        current = next;
    }
}

void clear_cmd_seqs(t_minishell *minishell)
{
    for (int i = 0; i < MAXPIPS; i++)
    {
        if (minishell->cmd_seqs[i])
        {
            free_token_list(minishell->cmd_seqs[i]);
            minishell->cmd_seqs[i] = NULL;
        }
    }
}

void clear_exec_seqs(t_minishell *minishell)
{
    for (int i = 0; i < MAXPIPS; i++)
    {
        if (minishell->exec_seqs[i])
        {
            free_token_list(minishell->exec_seqs[i]);
            minishell->exec_seqs[i] = NULL;
        }
    }
}

void cleanup_minishell(t_minishell *minishell)
{
    reset_minishell_args(minishell);
    mlstclear(minishell->exec_lst);
    mlstclear(minishell->tok_lst);
    ft_envlstclear(minishell);
	if(minishell->pids)
   		free(minishell->pids);

    ft_printf("Entering reset_sequences\n");
    clear_cmd_seqs(minishell);
    clear_exec_seqs(minishell);
}

// Initialize the minishell struct
void	initialize_minishell(t_minishell *minishell, char **envp)
{
	minishell->leave = 0;
	minishell->last_exitcode = 0;
	minishell->exitcode = 0;
	minishell->envp = envp;
	minishell->prompt = NULL;
}
