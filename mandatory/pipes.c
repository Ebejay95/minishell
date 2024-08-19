/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 09:33:43 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	initialize_cmd_seqs(t_minishell *m)
{
	int	i;

	i = 0;
	while (i < 1024)
		m->cmd_seqs[i++] = NULL;
}

void	add_to_temp_list(t_list **temp_list, t_token *token)
{
	t_token	*token_dup;
	t_list	*new_node;

	token_dup = duplicate(token);
	if (!token_dup)
		ft_error_exit("token_dup");
	new_node = ft_lstnew(token_dup);
	if (!new_node)
	{
		newfree_token(token_dup);
		ft_error_exit("ft_lstnew");
	}
	ft_lstadd_back(temp_list, new_node);
}

void	handle_pipe_token(t_minishell *m, t_list **temp_list, int *pipe_index)
{
	if (*temp_list != NULL)
	{
		m->cmd_seqs[(*pipe_index)++] = *temp_list;
		*temp_list = NULL;
	}
	m->pipes++;
}

void	split_pipes(t_minishell *m)
{
	t_list	*current;
	t_list	*temp_list;
	int		pipe_index;
	t_token	*token;

	initialize_cmd_seqs(m);
	temp_list = NULL;
	pipe_index = 0;
	m->pipes = 0;
	current = m->tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
			handle_pipe_token(m, &temp_list, &pipe_index);
		else
			add_to_temp_list(&temp_list, token);
		current = current->next;
	}
	if (temp_list != NULL)
		m->cmd_seqs[pipe_index] = temp_list;
}
