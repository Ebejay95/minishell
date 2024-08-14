/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 14:05:56 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static void	add_seq_to_cmd_seq(t_list ***cmd_seq, t_list *seq, int *count)
{
	t_list	**new_cmd_seq;
	int		i;

	new_cmd_seq = ft_calloc(*count + 2, sizeof(t_list *));
	if (!new_cmd_seq)
		ft_error_exit("ft_calloc");
	if (*cmd_seq)
	{
		i = 0;
		while (i < *count)
		{
			new_cmd_seq[i] = (*cmd_seq)[i];
			i++;
		}
		free(*cmd_seq);
	}
	new_cmd_seq[*count] = seq;
	(*count)++;
	*cmd_seq = new_cmd_seq;
}

void	split_pipes(t_minishell *m, t_list ***cmd_seq, t_list ***exec_seq)
{
	int		count;
	t_list	*current;
	t_list	*s;
	t_token	*token;

	count = 0;
	current = m->tok_lst;
	s = NULL;
	*cmd_seq = NULL;
	m->pipes = 0;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
		{
			if (s != NULL)
			{
				add_seq_to_cmd_seq(cmd_seq, s, &count);
				s = NULL;
			}
			m->pipes++;
		}
		else
		{
			if (s == NULL || (token->token == COMMAND && s->content != NULL))
			{
				if (s != NULL)
					add_seq_to_cmd_seq(cmd_seq, s, &count);
				s = NULL;
			}
			add_token_to_list(&s, token);
		}
		current = current->next;
	}
	if (s != NULL)
		add_seq_to_cmd_seq(cmd_seq, s, &count);
	*exec_seq = ft_calloc(count + 1, sizeof(t_list *));
	if (!*exec_seq)
		ft_error_exit("ft_calloc");
}
