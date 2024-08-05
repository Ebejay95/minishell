/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_inits.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:09:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/05 17:32:12 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

t_token	*init_pipe_details(t_token *pipetok)
{
	pipetok->detail.pipe.fdin = -1;
	pipetok->detail.pipe.fdout = -1;
	pipetok->detail.pipe.open_prompt = 0;
	return (pipetok);
}

t_token	*init_redirection_details(t_token *redirectiontoken)
{
	redirectiontoken->detail.rdrc.fdin = -1;
	redirectiontoken->detail.rdrc.fdout = -1;
	redirectiontoken->detail.rdrc.rdrctype = NULL;
	return (redirectiontoken);
}

void	init_check_rdrc(t_list *last, char *last_type, char *last_str)
{
	last = NULL;
	last_type = NULL;
	last_str = NULL;
}

void	set_rdrctype(t_list *last, t_list *current, t_token *token)
{
	check_semantics(last, current);
	if (ft_strcmp(token->type, "Redirection") == 0)
	{
		if (ft_strcmp(token->str, "<<") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("here_doc");
		if (ft_strcmp(token->str, ">>") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("append");
		if (ft_strcmp(token->str, "<") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("redirection");
		if (ft_strcmp(token->str, ">") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("truncate");
	}
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
