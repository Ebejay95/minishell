/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:11:51 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 10:17:38 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	set_token_expmap(t_token *newtok, char *expmap)
{
	newtok->expmap = ft_strdup(expmap);
	if (!newtok->expmap)
		return (0);
	return (1);
}

void	free_token_resources(t_token *newtok)
{
	if (newtok->str)
		free(newtok->str);
	free(newtok);
}

t_token	*create_token(char *str, char *expmap)
{
	t_token	*newtok;

	if (!validate_input(str, expmap))
		return (NULL);
	newtok = allocate_token();
	if (!newtok)
		return (NULL);
	if (!set_token_str(newtok, str))
	{
		free_token_resources(newtok);
		return (NULL);
	}
	set_token_details(newtok, str);
	if (!set_token_expmap(newtok, expmap))
	{
		free_token_resources(newtok);
		if (str)
			free(str);
		return (NULL);
	}
	return (newtok);
}

void	print_toktype(enum e_toktype token)
{
	if (token == REDIRECTION)
		ft_printf("Redirection");
	else if (token == PIPE)
		ft_printf("Pipe");
	else if (token == COMMAND)
		ft_printf("Command");
	else if (token == WORD)
		ft_printf("Word");
	else if (token == MINIFILE)
		ft_printf("File");
	else if (token == DELIMITER)
		ft_printf("Delimiter");
	else if (token == UNSET)
		ft_printf("UNSET");
	else
		ft_printf("Unknown");
}

void	put_token_details(t_token *token)
{
	if (token->token == PIPE)
	{
		ft_printf(" (open_prompt: %d)", token->open_prompt);
	}
	else if (token->token == REDIRECTION)
	{
		ft_printf(" ( rdrcmeta: %s %p rdrctarget: %s %p)",
			token->rdrcmeta,
			token->rdrcmeta,
			token->rdrctarget,
			token->rdrctarget);
	}
	else if (token->token == MINIFILE)
	{
		ft_printf(" (fd: %d)", token->fd);
	}
}
