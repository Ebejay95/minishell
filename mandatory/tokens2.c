/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:46:49 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 14:03:21 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	free_token(void *n)
{
	t_token	*token;

	if (!n)
		return ;
	token = (t_token *)n;
	if (token->is_freed)
		return ;

	if (token->str)
	{
		free(token->str);
		token->str = NULL;
	}
	if (token->type)
	{
		free(token->type);
		token->type = NULL;
	}
	if (token->detail.rdrc.rdrcmeta)
	{
		free(token->detail.rdrc.rdrcmeta);
		token->detail.rdrc.rdrcmeta = NULL;
	}
	if (token->detail.rdrc.rdrctarget)
	{
		free(token->detail.rdrc.rdrctarget);
		token->detail.rdrc.rdrctarget = NULL;
	}
	if (token->expmap)
	{
		free(token->expmap);
		token->expmap = NULL;
	}
	token->is_freed = 1;
}

void	update_tok_type(t_token *tok, enum e_toktype token)
{
	tok->token = token;
	tok->type = toktype_to_str(token);
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
		tok->type = toktype_to_str(token);
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
	tok->type = toktype_to_str(token);
}
