/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_prexecute.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:40:28 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 00:33:26 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

t_token	*duplicate(t_token *src)
{
	t_token	*ne;

	ne = malloc(sizeof(t_token));
	if (!ne)
		return (NULL);
	*ne = *src;
	ne->str = ft_strdup(src->str);
	ne->expmap = ft_strdup(src->expmap);
	if (src->rdrcmeta)
		ne->rdrcmeta = ft_strdup(src->rdrcmeta);
	else
		ne->rdrcmeta = NULL;
	if (src->rdrctarget)
		ne->rdrctarget = ft_strdup(src->rdrctarget);
	else
		ne->rdrctarget = NULL;
	if (!ne->str || !ne->expmap
		|| (src->rdrcmeta && !ne->rdrcmeta)
		|| (src->rdrctarget && !ne->rdrctarget))
	{
		free(ne->expmap);
		free(ne->rdrcmeta);
		return (free(ne->str), free(ne->rdrctarget), free(ne), NULL);
	}
	return (ne);
}

static void	prex_ins(t_minishell *m, t_list *tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;
	t_token	*ne;

	current = tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (!ft_strcmp(token->str, "<<"))
		{
			handle_heredoc(m, current);
			ne = duplicate(token);
			ft_lstadd_back(exec_lst, ft_lstnew(ne));
		}
		else if (!ft_strcmp(token->str, "<"))
		{
			handle_infile(current);
			ne = duplicate(token);
			ft_lstadd_back(exec_lst, ft_lstnew(ne));
		}
		current = current->next;
	}
}

static void	prex_mains(t_list *tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;
	t_token	*ne;

	current = tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (keep_for_exec(token))
		{
			ne = duplicate(token);
			ft_lstadd_back(exec_lst, ft_lstnew(ne));
		}
		current = current->next;
	}
}

static void	prex_outs(t_list *tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;
	t_token	*ne;

	current = tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (!ft_strcmp(token->str, ">") || !ft_strcmp(token->str, ">>"))
		{
			handle_trunc_append(current);
			ne = duplicate(token);
			ft_lstadd_back(exec_lst, ft_lstnew(ne));
		}
		current = current->next;
	}
}

void	prexecute(t_minishell *m, int i)
{
	if (m->exec_lst)
	{
		mlstclear(m->exec_lst);
		m->exec_lst = NULL;
	}
	if (m->pipes == 0)
	{
		prex_ins(m, m->tok_lst, &(m->exec_lst));
		prex_mains(m->tok_lst, &(m->exec_lst));
		prex_outs(m->tok_lst, &(m->exec_lst));
	}
	else
	{
		prex_ins(m, m->cmd_seqs[i], &(m->exec_seqs[i]));
		prex_mains(m->cmd_seqs[i], &(m->exec_seqs[i]));
		prex_outs(m->cmd_seqs[i], &(m->exec_seqs[i]));
	}
}
