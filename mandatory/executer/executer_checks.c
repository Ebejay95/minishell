/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:04:35 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 12:28:47 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static void	check_rdrc_norm(char *last_str, t_token *cont, t_minishell *m)
{
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, ">"))
		pic_err(m, 2, SYN_ERR_GT);
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, ">>"))
		pic_err(m, 2, SYN_ERR_DBL_GT);
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, "<"))
		pic_err(m, 2, SYN_ERR_LT);
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, "<<"))
		pic_err(m, 2, SYN_ERR_DBL_LT);
}

static void	check_pipes(t_minishell *m)
{
	t_list	*cur;
	t_token	*cont;
	char	*end;
	char	*ltype;

	end = NULL;
	ltype = NULL;
	cur = m->tok_lst;
	while (cur != NULL)
	{
		cont = (t_token *)cur->content;
		if (check_one(m, ltype, cont->type))
			pic_err(m, 2, SYN_ERR_PIPE);
		else if (check_two(m, cont->type, end))
			pic_err(m, 2, SYN_ERR_PIPE);
		else if (check_three(m, cur->next, cont->type))
			pic_err(m, 2, SYN_ERR_NEWLINE);
		end = cont->type;
		ltype = cont->type;
		cur = cur->next;
	}
}

static void	check_rdrcmeta(t_list *last, t_list *current, t_token *token)
{
	init_semantics(last, current);
	if (ft_strcmp(token->type, "Redirection") == 0)
	{
		if (ft_strcmp(token->str, "<<") == 0)
			token->detail.rdrc.rdrcmeta = ft_strdup("here_doc");
		if (ft_strcmp(token->str, ">>") == 0)
		{
			token->detail.rdrc.rdrcmeta = ft_strdup("append");
			update_tok_type_next_word(current, MINIFILE);
		}
		if (ft_strcmp(token->str, "<") == 0)
		{
			token->detail.rdrc.rdrcmeta = ft_strdup("redirection");
			update_tok_type_next_word(current, MINIFILE);
		}
		if (ft_strcmp(token->str, ">") == 0)
		{
			token->detail.rdrc.rdrcmeta = ft_strdup("truncate");
			update_tok_type_next_word(current, MINIFILE);
		}
	}
}

static void	check_redirections(t_minishell *m)
{
	t_list	*cur;
	t_list	*last;
	t_token	*cont;
	char	*end;
	char	*last_str;

	last = NULL;
	end = NULL;
	last_str = NULL;
	cur = m->tok_lst;
	while (cur != NULL)
	{
		cont = (t_token *)cur->content;
		if (check_four(m, cont->type, end))
			pic_err(m, 2, SYN_ERR_PIPE);
		else if (check_five(m, cur->next, cont->type))
			pic_err(m, 2, SYN_ERR_NEWLINE);
		else if (check_six(m, end, cont->type))
			check_rdrc_norm(last_str, cont, m);
		check_rdrcmeta(last, cur, cont);
		end = cont->type;
		last_str = cont->str;
		last = cur;
		cur = cur->next;
	}
}

void	pre_exec_checks(t_minishell *m)
{
	if (!m->leave)
		check_pipes(m);
	if (!m->leave)
		check_redirections(m);
}
