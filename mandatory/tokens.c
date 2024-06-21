/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:12:44 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/20 11:16:16 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// create a token from lexer data
t_token	*create_token(enum e_toktype token)
{
	t_token		*newtok;

	newtok = (t_token *)malloc(sizeof(t_token));
	if (!newtok)
		return (NULL);
	newtok->token = token;
	newtok->type = toktype_to_str(token);
	return (newtok);
}

// tranform token type into string for readeable cmp and logging
char	*toktype_to_str(enum e_toktype token)
{
	if (token == REDIRECTION)
		return (ft_strdup("Redirection"));
	if (token == PIPE)
		return (ft_strdup("Pipe"));
	if (token == COMMAND)
		return (ft_strdup("Command"));
	if (token == ARGUMENT)
		return (ft_strdup("Argument"));
	return (ft_strdup(""));
}

// print the content of a grammar token 
void	put_token(void *content)
{
	t_token	*astitem;

	astitem = (t_token *)content;
	if (astitem != NULL)
	{
		ft_printf(Y"["D);
		ft_printf("%s", astitem->type);
		ft_printf(Y"]\n"D);
	}
	else
	{
		ft_printf(Y"["D);
		ft_printf("(null)");
		ft_printf(Y"]\n"D);
	}
}
