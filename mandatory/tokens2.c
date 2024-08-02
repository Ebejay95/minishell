/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:46:49 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/02 12:32:57 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	free_token(void *n)
{
	t_token	*token;

	if (n)
	{
		token = (t_token *)n;
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
		free(token);
		token = NULL;
	}
}

void	update_tok_type(t_token *tok, enum e_toktype token)
{
	tok->token = token;
	tok->type = toktype_to_str(token);
}
