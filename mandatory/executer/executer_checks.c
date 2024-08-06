/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:04:35 by chorst            #+#    #+#             */
/*   Updated: 2024/08/06 19:22:39 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

// # USED TEXT MESSAGES:
// 3: bash: syntax error near unexpected token `>'
// 4: bash: syntax error near unexpected token `>>'
// 5: bash: syntax error near unexpected token `<'
// 6: bash: syntax error near unexpected token `<<'
static void	check_rdrc_norm(char *last_str, t_token *cont, t_minishell *m)
{
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, ">"))
		pic_err(m, 2, text(3));
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, ">>"))
		pic_err(m, 2, text(4));
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, "<"))
		pic_err(m, 2, text(5));
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, "<<"))
		pic_err(m, 2, text(6));
}

// # USED TEXT MESSAGES:
// #2: bash: syntax error near unexpected token `|'
// #8: Pipe
static void	check_pipes(t_minishell *m)
{
	t_list	*current;
	t_token	*cont;
	char	*end;
	char	*last_str;
	char	*ltype;

	end = NULL;
	last_str = NULL;
	ltype = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cont = (t_token *)current->content;
		if (ft_strcmp(cont->type, text(8)) == 0 && end == NULL)
			pic_err(m, 2, text(2));
		else if (current->next == NULL && !ft_strcmp(cont->type, text(8)))
			pic_err(m, 2, text(1));
		else if (!ft_strcmp(ltype, text(8)) && !ft_strcmp(cont->type, text(8)))
			pic_err(m, 2, text(2));
		end = cont->type;
		last_str = cont->str;
		ltype = cont->type;
		current = current->next;
	}
}

// # USED TEXT MESSAGES:
// #1: bash: syntax error near unexpected token `newline'
// #2: bash: syntax error near unexpected token `|'
// #7: Redirection
// #8: Pipe
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
		if (!ft_strcmp(cont->type, text(8)) && !ft_strcmp(end, text(7)))
			pic_err(m, 2, text(2));
		else if (cur->next == NULL && !ft_strcmp(cont->type, text(7)))
			pic_err(m, 2, text(1));
		else if (!ft_strcmp(end, text(7)) && !ft_strcmp(cont->type, text(7)))
			check_rdrc_norm(last_str, cont, m);
		set_rdrctype(last, cur, cont);
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

void	check_semantics(t_list *last, t_list *current)
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
			update_tok_type(token, DELIMITER);
		if (!ft_strcmp(token->type, "Word")
			&& !ft_strcmp(last_token->type, "Pipe")
			&& !ft_strcmp(last_token->str, "|"))
			update_tok_type(token, COMMAND);
	}
	else
		if (!ft_strcmp(token->type, "Word"))
			update_tok_type(token, COMMAND);
}
