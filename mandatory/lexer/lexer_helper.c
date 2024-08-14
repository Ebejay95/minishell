/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:54:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 21:06:37 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	detect_lexing_errors(t_minishell *m)
{
	char	*work;
	char	quote_char;
	int		in_quote;

	work = m->prompt;
	quote_char = 0;
	in_quote = 0;
	while (*work != '\0')
	{
		if (!in_quote && (*work == '\'' || *work == '\"'))
		{
			quote_char = *work;
			in_quote = 1;
		}
		else if (in_quote && *work == quote_char)
			in_quote = 0;
		work++;
	}
	if (in_quote)
		pic_err(m, 2, "unclosed quotes");
}

void	add_token_to_list(t_list **lst, t_token *token)
{
	t_list	*new_element;
	t_list	*current;

	if (token)
	{
		new_element = ft_lstnew((void *)token);
		if (!new_element)
		{
			free_token(token);
			return ;
		}
		if (*lst == NULL)
			*lst = new_element;
		else
		{
			current = *lst;
			while (current->next != NULL)
				current = current->next;
			current->next = new_element;
		}
	}
}
