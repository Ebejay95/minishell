/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:46:49 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 10:12:19 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	free_if_not_null(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_token(void *n)
{
	t_token	*token;

	token = (t_token *)n;
	if (!token || token->is_freed)
		return ;
	free_if_not_null((void **)&token->str);
	free_if_not_null((void **)&token->rdrcmeta);
	free_if_not_null((void **)&token->rdrctarget);
	free_if_not_null((void **)&token->expmap);
	token->is_freed = 1;
}

void	update_tok_type(t_token *tok, enum e_toktype token)
{
	tok->token = token;
}

void	update_tok_type_next_word(t_list *current, enum e_toktype token)
{
	t_token	*tok;

	if (current->next != NULL)
	{
		current = current->next;
	}
	tok = (t_token *)current->content;
	if (tok->token == WORD)
	{
		tok->token = token;
	}
}

void	update_tok_type_next(t_list *current, enum e_toktype token)
{
	t_token	*tok;

	if (current->next != NULL)
	{
		current = current->next;
	}
	tok = (t_token *)current->content;
	tok->token = token;
}
