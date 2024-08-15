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

int	set_token_expmap(t_token *newtok, const char *expmap)
{
	newtok->expmap = ft_strdup(expmap);
	if (!newtok->expmap)
		return (0);
	return (1);
}

void	free_token_resources(t_token *newtok)
{
	if (newtok->type)
		free(newtok->type);
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
	if (!set_token_type(newtok) || !set_token_str(newtok, str))
	{
		free_token_resources(newtok);
		return (NULL);
	}
	set_token_details(newtok, str);
	if (!set_token_expmap(newtok, expmap))
	{
		free_token_resources(newtok);
		return (NULL);
	}
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
	if (token == WORD)
		return (ft_strdup("Word"));
	if (token == MINIFILE)
		return (ft_strdup("File"));
	if (token == DELIMITER)
		return (ft_strdup("Delimiter"));
	if (token == UNSET)
		return (ft_strdup("UNSET"));
	return (ft_strdup(""));
}

void	put_token_details(t_token *token)
{
	if (ft_strcmp(token->type, "Pipe") == 0)
	{
		ft_printf(" (open_prompt: %d)", token->detail.pipe.open_prompt);
	}
	else if (ft_strcmp(token->type, "Redirection") == 0)
	{
		ft_printf(" ( rdrcmeta: %s rdrctarget: %s)",
			token->detail.rdrc.rdrcmeta,
			token->detail.rdrc.rdrctarget);
	}
	else if (ft_strcmp(token->type, "File") == 0)
	{
		ft_printf(" (fd: %d)", token->detail.minifile.fd);
	}
}
