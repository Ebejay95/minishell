/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 07:28:45 by jonathanebe      ###   ########.fr       */
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

	ft_printf("DEBUG: Entering expand_toklst\n");
	current = *tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		ft_printf("DEBUG: Expanding token: %s\n", cur_content->str);
		expand_token(m, cur_content);
		ft_printf("DEBUG: After expansion: %s\n", cur_content->str);
		current = current->next;
	}
	ft_printf("DEBUG: Exiting expand_toklst\n");
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
