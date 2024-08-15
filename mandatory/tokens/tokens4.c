/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:17:10 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 10:17:53 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	put_token(void *content)
{
	t_token		*token;

	token = (t_token *)content;
	if (token != NULL)
	{
		ft_printf(Y"["D);
		ft_printf(C"%s$ "D, token->type);
		ft_printf("%s$ ", token->str);
		ft_printf("map: %s", token->expmap);
		put_token_details(token);
		ft_printf(Y"]"D);
	}
	else
	{
		ft_printf(Y"["D);
		ft_printf("(null)");
		ft_printf(Y"]"D);
	}
}

t_token	tok_lst_get(void *n)
{
	return (*(t_token *)n);
}
