/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 07:36:20 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	split_pipes(t_minishell *m)
{
	t_list	*current;
	t_list	*temp_list;
	int		pipe_index;
	t_token	*token;
	t_token	*token_dup;
	t_list	*new_node;
	int		i;

	temp_list = NULL;
	pipe_index = 0;
	i = 0;
	while (i < 1024)
	{
		m->cmd_seqs[i] = NULL;
		i++;
	}
	current = m->tok_lst;
	m->pipes = 0;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
		{
			if (temp_list != NULL)
			{
				m->cmd_seqs[pipe_index] = temp_list;
				temp_list = NULL;
				pipe_index++;
			}
			m->pipes++;
		}
		else
		{
			token_dup = duplicate(token);
			if (!token_dup)
				ft_error_exit("token_dup");
			new_node = ft_lstnew(token_dup);
			if (!new_node)
			{
				newfree_token(token_dup);
				ft_error_exit("ft_lstnew");
			}
			ft_lstadd_back(&temp_list, new_node);
		}
		current = current->next;
	}
	if (temp_list != NULL)
		m->cmd_seqs[pipe_index] = temp_list;
}
