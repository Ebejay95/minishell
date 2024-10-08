/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:04:35 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 00:56:06 by jonathanebe      ###   ########.fr       */
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
	t_list			*cur;
	t_token			*cont;
	enum e_toktype	end;
	enum e_toktype	ltype;

	end = UNSET;
	ltype = UNSET;
	cur = m->tok_lst;
	while (cur != NULL)
	{
		cont = (t_token *)cur->content;
		if (check_one(m, ltype, cont->token))
			pic_err(m, 2, SYN_ERR_PIPE);
		else if (check_two(m, cont->token, end))
			pic_err(m, 2, SYN_ERR_PIPE);
		else if (check_three(m, cur->next, cont->token))
			pic_err(m, 2, SYN_ERR_NEWLINE);
		end = cont->token;
		ltype = cont->token;
		cur = cur->next;
	}
}

static void	check_rdrcmeta(t_list *last, t_list *current, t_token *token)
{
	init_semantics(last, current);
	if (token->token == REDIRECTION)
	{
		if (ft_strcmp(token->str, "<<") == 0)
			token->rdrcmeta = ft_strdup("here_doc");
		if (ft_strcmp(token->str, ">>") == 0)
		{
			token->rdrcmeta = ft_strdup("append");
			update_tok_type_next_word(current, MINIFILE);
		}
		if (ft_strcmp(token->str, "<") == 0)
		{
			token->rdrcmeta = ft_strdup("redirection");
			update_tok_type_next_word(current, MINIFILE);
		}
		if (ft_strcmp(token->str, ">") == 0)
		{
			token->rdrcmeta = ft_strdup("truncate");
			update_tok_type_next_word(current, MINIFILE);
		}
	}
}

static void	check_redirections(t_minishell *m)
{
	t_list			*cur;
	t_list			*last;
	t_token			*cont;
	enum e_toktype	end;
	char			*last_str;

	last = NULL;
	end = UNSET;
	last_str = NULL;
	cur = m->tok_lst;
	while (cur != NULL)
	{
		cont = (t_token *)cur->content;
		if (check_four(m, cont->token, end))
			pic_err(m, 2, SYN_ERR_PIPE);
		else if (check_five(m, cur->next, cont->token))
			pic_err(m, 2, SYN_ERR_NEWLINE);
		else if (check_six(m, end, cont->token))
			check_rdrc_norm(last_str, cont, m);
		check_rdrcmeta(last, cur, cont);
		end = cont->token;
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
