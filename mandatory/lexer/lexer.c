/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 09:55:32 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	add_token_to_list(t_list **lst, t_token *token)
{
	ft_lstadd_back(lst, ft_lstnew(token));
}

void	expand_toklst(t_minishell *m, t_list **tok_lst)
{
	t_list	*current;
	t_token	*cur_content;

	current = *tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		expand_token(m, cur_content);
		current = current->next;
	}
}

void	lex_prompt(t_minishell *m)
{
	detect_lexing_errors(m);
	prompt_to_token(m);
	if (DEBUG == 1)
	{
		ft_printf(Y"TOKENs:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	expand_toklst(m, &(m->tok_lst));
	if (DEBUG == 1)
	{
		ft_printf(Y"TOKENLIST:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	afterbreakup(&(m->tok_lst));
}
