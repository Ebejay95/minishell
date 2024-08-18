/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_inits.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:09:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 14:27:37 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static t_token	*init_redirection_details(t_token *redirectiontoken)
{
	redirectiontoken->rdrcmeta = NULL;
	redirectiontoken->rdrctarget = NULL;
	return (redirectiontoken);
}

void	init_semantics(t_list *last, t_list *current)
{
	t_token	*last_token;
	t_token	*token;

	last_token = NULL;
	token = (t_token *)current->content;
	if (last)
	{
		last_token = (t_token *)last->content;
		if (token->token == WORD
			&& last_token->token == REDIRECTION
			&& !ft_strcmp(last_token->str, "<<"))
		{
			update_tok_type(token, DELIMITER);
			update_tok_type_next_word(current, COMMAND);
		}
		if (token->token == WORD
			&& last_token->token == PIPE
			&& !ft_strcmp(last_token->str, "|"))
			update_tok_type(token, COMMAND);
	}
	else
		if (token->token == WORD)
			update_tok_type(token, COMMAND);
}

void	pre_exec_prep(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (cur_content->token == REDIRECTION)
			init_redirection_details(cur_content);
		current = current->next;
	}
}

void	init_fd(t_fd *fd, int input_fd, int output_fd)
{
	fd->input = input_fd;
	fd->output = output_fd;
	fd->last_input = input_fd;
	fd->last_output = output_fd;
}
