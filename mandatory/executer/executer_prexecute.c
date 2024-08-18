/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_prexecute.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:40:28 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 18:49:01 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

t_token *duplicate(t_token *src)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;

    *new_token = *src;
    new_token->str = ft_strdup(src->str);
    new_token->expmap = ft_strdup(src->expmap);
    new_token->rdrcmeta = src->rdrcmeta ? ft_strdup(src->rdrcmeta) : NULL;
    new_token->rdrctarget = src->rdrctarget ? ft_strdup(src->rdrctarget) : NULL;

    if (!new_token->str || !new_token->expmap || 
        (src->rdrcmeta && !new_token->rdrcmeta) || 
        (src->rdrctarget && !new_token->rdrctarget))
    {
        free(new_token->str);
        free(new_token->expmap);
        free(new_token->rdrcmeta);
        free(new_token->rdrctarget);
        free(new_token);
        return NULL;
    }

    return new_token;
}

static void prex_ins(t_minishell *m, t_list *tok_lst, t_list **exec_lst)
{
    t_list *current;
    t_token *token;
    t_token *new_token;

    current = tok_lst;
    while (current != NULL)
    {
        token = (t_token *)current->content;
        if (!ft_strcmp(token->str, "<<"))
        {
            handle_heredoc(m, current);
            new_token = duplicate(token);
            ft_lstadd_back(exec_lst, ft_lstnew(new_token));
        }
        else if (!ft_strcmp(token->str, "<"))
        {
            handle_infile(current);
            new_token = duplicate(token);
            ft_lstadd_back(exec_lst, ft_lstnew(new_token));
        }
        current = current->next;
    }
}

static void prex_mains(t_list *tok_lst, t_list **exec_lst)
{
    t_list *current;
    t_token *token;
    t_token *new_token;

    current = tok_lst;
    while (current != NULL)
    {
        token = (t_token *)current->content;
        if (keep_for_exec(token))
        {
            new_token = duplicate(token);
            ft_lstadd_back(exec_lst, ft_lstnew(new_token));
        }
        current = current->next;
    }
}

static void prex_outs(t_list *tok_lst, t_list **exec_lst)
{
    t_list *current;
    t_token *token;
    t_token *new_token;

    current = tok_lst;
    while (current != NULL)
    {
        token = (t_token *)current->content;
        if (!ft_strcmp(token->str, ">") || !ft_strcmp(token->str, ">>"))
        {
            handle_trunc_append(current);
            new_token = duplicate(token);
            ft_lstadd_back(exec_lst, ft_lstnew(new_token));
        }
        current = current->next;
    }
}

void prexecute(t_minishell *m, int i)
{
    // Prüfen und Freigeben von m->exec_lst vor neuer Zuweisung
    if (m->exec_lst)
    {
        mlstclear(m->exec_lst);
        m->exec_lst = NULL;
    }
    ft_printf("prexecute start\n");
	if (m->pipes == 0)
	{
    	ft_printf("tok_lst:\n");
    	ft_lstput(&(m->tok_lst), put_token, '\n');
	prex_ins(m, m->tok_lst, &(m->exec_lst));
	prex_mains(m->tok_lst, &(m->exec_lst));
	prex_outs(m->tok_lst, &(m->exec_lst));
    	ft_printf("exec_lst:\n");
	ft_lstput(&(m->exec_lst), put_token, '\n');
	}
	else
	{
    	ft_printf("tok_lst:\n");
    	ft_lstput(&(m->tok_lst), put_token, '\n');
    	ft_printf("cmd_seqs_lst:\n");
    	ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
		prex_ins(m, m->cmd_seqs[i], &(m->exec_seqs[i]));
		prex_mains(m->cmd_seqs[i], &(m->exec_seqs[i]));
		prex_outs(m->cmd_seqs[i], &(m->exec_seqs[i]));
	}
}