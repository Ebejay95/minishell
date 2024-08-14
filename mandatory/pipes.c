/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 14:31:09 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static void	add_seq_to_cmd_seq(t_pipe_data *data)
{
	t_list	**new_cmd_seq;
	int		i;

	new_cmd_seq = ft_calloc(*(data->count) + 2, sizeof(t_list *));
	if (!new_cmd_seq)
		ft_error_exit("ft_calloc");
	if (*(data->cmd_seq))
	{
		i = 0;
		while (i < *(data->count))
		{
			new_cmd_seq[i] = (*(data->cmd_seq))[i];
			i++;
		}
		free(*(data->cmd_seq));
	}
	new_cmd_seq[*(data->count)] = *(data->s);
	(*(data->count))++;
	*(data->cmd_seq) = new_cmd_seq;
}

void	process_pipetoken(t_pipe_data *d, t_token *t)
{
	if (t->token == PIPE)
	{
		if (*(d->s) != NULL)
		{
			add_seq_to_cmd_seq(d);
			*(d->s) = NULL;
		}
		(*(d->pipes))++;
	}
	else
	{
		if (*(d->s) == NULL || (t->token == COMMAND && (*(d->s))->content != NULL))
		{
			if (*(d->s) != NULL)
				add_seq_to_cmd_seq(data);
			*(d->s) = NULL;
		}
		add_token_to_list(d->s, t);
	}
}

void	split_pipes(t_minishell *m, t_list ***cmd_seq, t_list ***exec_seq)
{
	t_pipe_data	data;
	t_list		*current;
	t_list		*s;
	int			count;

	count = 0;
	s = NULL;
	*cmd_seq = NULL;
	m->pipes = 0;
	data = (t_pipe_data){cmd_seq, &s, &count, &(m->pipes)};
	current = m->tok_lst;
	while (current != NULL)
	{
		process_pipetoken(&data, (t_token *)current->content);
		current = current->next;
	}
	if (s != NULL)
		add_seq_to_cmd_seq(&data);
	*exec_seq = ft_calloc(count + 1, sizeof(t_list *));
	if (!*exec_seq)
		ft_error_exit("ft_calloc");
}
