/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:12:44 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/28 20:28:07 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// create a token from lexer data
// t_token	*create_token(enum e_toktype token)
// {
// 	t_token		*newtok;

// 	newtok = (t_token *)malloc(sizeof(t_token));
// 	if (!newtok)
// 		return (NULL);
// 	newtok->token = token;
// 	newtok->type = toktype_to_str(token);
// 	return (newtok);
// }
t_token	*create_token(char *str, int start, int end, int expand)
{
	t_token		*newtok;

	newtok = (t_token *)malloc(sizeof(t_token));
	if (!newtok)
		return (NULL);
	newtok->token = UNSET;
	newtok->type = toktype_to_str(UNSET);
	newtok->str = str;
	newtok->start = start;
	newtok->end = end;
	newtok->expand = expand;
	return (newtok);
}

void	update_tok_type(t_token *tok, enum e_toktype token)
{
	tok->token = token;
	tok->type = toktype_to_str(token);
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
	if (token == UNSET)
		return (ft_strdup("UNSET"));
	return (ft_strdup(""));
}

// print the content of a grammar token 
void	put_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token != NULL)
	{
		ft_printf(Y"["D);
		ft_printf(C"%s "D, token->type);
		ft_printf("%s", token->str);
		if (token->expand)
			ft_printf(B " EXPAND"D, token->str);
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
