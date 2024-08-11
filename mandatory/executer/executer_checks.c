/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:04:35 by chorst            #+#    #+#             */
/*   Updated: 2024/08/11 20:46:10 by jeberle          ###   ########.fr       */
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
	t_list	*cur;
	t_token	*cont;
	char	*end;
	char	*last_str;
	char	*ltype;

	end = NULL;
	last_str = NULL;
	ltype = NULL;
	cur = m->tok_lst;
	while (cur != NULL)
	{
		cont = (t_token *)cur->content;
		if (check_one(m, ltype, cont->type))
			pic_err(m, 2, text(2));
		else if (check_two(m, cont->type, end))
			pic_err(m, 2, text(2));
		else if (check_three(m, cur->next, cont->type))
			pic_err(m, 2, text(1));
		end = cont->type;
		last_str = cont->str;
		ltype = cont->type;
		cur = cur->next;
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
		if (check_four(m, cont->type, end))
			pic_err(m, 2, text(2));
		else if (check_five(m, cur->next, cont->type))
			pic_err(m, 2, text(1));
		else if (check_six(m, end, cont->type))
			check_rdrc_norm(last_str, cont, m);
		set_rdrcmeta(last, cur, cont);
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
