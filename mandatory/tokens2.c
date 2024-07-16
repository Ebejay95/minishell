/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:46:49 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/16 20:47:09 by jeberle          ###   ########.fr       */
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
