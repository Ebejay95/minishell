/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:12:44 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/25 05:04:47 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

t_token	*create_token(char *str, char *expmap)
{
	t_token	*newtok;

	if (!str || !*str || !expmap || !*expmap)
		return (NULL);
	newtok = (t_token *)malloc(sizeof(t_token));
	if (!newtok)
		return (NULL);
	newtok->token = WORD;
	newtok->type = toktype_to_str(WORD);
	newtok->str = ft_strdup(str);
	if (!newtok->str)
	{
		free(newtok->type);
		free(newtok);
		return (NULL);
	}
	newtok->expmap = ft_strdup(expmap);
	if (!newtok->expmap)
	{
		free(newtok->type);
		free(newtok);
		return (NULL);
	}
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
	if (token == WORD)
		return (ft_strdup("Word"));
	if (token == UNSET)
		return (ft_strdup("UNSET"));
	return (ft_strdup(""));
}

void	put_token(void *content)
{
	t_token		*token;
	int			i;

	i = 0;
	token = (t_token *)content;
	if (token != NULL)
	{
		ft_printf(Y"["D);
		ft_printf(C"%s "D, token->type);
		ft_printf("%s", token->str);
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
