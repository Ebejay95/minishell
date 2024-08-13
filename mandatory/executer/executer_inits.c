/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_inits.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:09:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 16:15:59 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static t_token	*init_pipe_details(t_token *pipetok)
{
	pipetok->detail.pipe.open_prompt = 0;
	return (pipetok);
}

static t_token	*init_redirection_details(t_token *redirectiontoken)
{
	redirectiontoken->detail.rdrc.rdrcmeta = NULL;
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
		if (!ft_strcmp(token->type, "Word")
			&& !ft_strcmp(last_token->type, "Redirection")
			&& !ft_strcmp(last_token->str, "<<"))
		{
			update_tok_type(token, DELIMITER);
			update_tok_type_next_word(current, COMMAND);
		}
		if (!ft_strcmp(token->type, "Word")
			&& !ft_strcmp(last_token->type, "Pipe")
			&& !ft_strcmp(last_token->str, "|"))
			update_tok_type(token, COMMAND);
	}
	else
		if (!ft_strcmp(token->type, "Word"))
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
		if (ft_strcmp(cur_content->type, "Pipe") == 0)
			cur_content = init_pipe_details(cur_content);
		else if (ft_strcmp(cur_content->type, "Redirection") == 0)
			init_redirection_details(cur_content);
		current = current->next;
	}
}
