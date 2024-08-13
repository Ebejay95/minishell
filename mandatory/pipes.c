/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/12 16:41:47 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static t_list	**add_seq_to_cmd_seq(t_list **cmd_seq, t_list *seq, int *count)
{
	cmd_seq = ft_realloc(cmd_seq, sizeof(t_list *) * (*count + 1));
	if (cmd_seq == NULL)
		ft_error_exit("ft_realloc");
	cmd_seq[(*count)++] = seq;
	return (cmd_seq);
}

// Hauptlogik für die Verarbeitung der Token
static t_list	**process_tokens(t_minishell *m, t_list *current, int *count)
{
	t_list	*seq;
	t_list	**cmd_seq;
	t_token	*token;

	seq = NULL;
	cmd_seq = NULL;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
		{
			if (seq != NULL)
			{
				cmd_seq = add_seq_to_cmd_seq(cmd_seq, seq, count);
				seq = NULL;
			}
			m->pipes++;
		}
		else
			add_token_to_list(&seq, token);
		current = current->next;
	}
	if (seq != NULL)
		cmd_seq = add_seq_to_cmd_seq(cmd_seq, seq, count);
	return (cmd_seq);
}

// Hauptfunktion
void	split_pipes(t_minishell *m, t_list ***cmd_seq, t_list ***exec_seq)
{
	int	count;
	t_list	**new_cmd_seq;
	t_list	**new_exec_seq;

	count = 0;
	*cmd_seq = process_tokens(m, m->tok_lst, &count);
	new_cmd_seq = ft_calloc(count + 1, sizeof(t_list *));
	if (new_cmd_seq == NULL)
		ft_error_exit("ft_calloc");
	ft_memcpy(new_cmd_seq, *cmd_seq, count * sizeof(t_list *));
	free(*cmd_seq);
	*cmd_seq = new_cmd_seq;
	new_exec_seq = ft_calloc(count + 1, sizeof(t_list *));
	if (new_exec_seq == NULL)
	{
		free(*cmd_seq);
		ft_error_exit("ft_calloc");
	}
	if (*exec_seq != NULL)
	{
		ft_memcpy(new_exec_seq, *exec_seq, count * sizeof(t_list *));
		free(*exec_seq);
	}
	*exec_seq = new_exec_seq;
	m->pipes = count - 1;  // Setze die Anzahl der Pipes
}
