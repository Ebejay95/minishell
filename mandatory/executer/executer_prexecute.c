/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_prexecute.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:40:28 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 14:25:00 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static void	prex_ins(t_minishell *m, t_list **tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;

	current = *tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (!ft_strcmp(token->str, "<<"))
		{
			handle_heredoc(m, current);
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		else if (!ft_strcmp(token->str, "<"))
		{
			handle_infile(current);
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		current = current->next;
	}
}

static void	prex_mains(t_list **tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;

	current = *tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (keep_for_exec(token))
		{
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		current = current->next;
	}
}

static void	prex_outs(t_list **tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;

	current = *tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (!ft_strcmp(token->str, ">") || !ft_strcmp(token->str, ">>"))
		{
			handle_trunc_append(current);
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		current = current->next;
	}
}

void	prexecute(t_minishell *m, t_list **tok_lst, t_list **exec_lst)
{
	prex_ins(m, tok_lst, exec_lst);
	prex_mains(tok_lst, exec_lst);
	prex_outs(tok_lst, exec_lst);
}
