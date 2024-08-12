/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:12:44 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/11 17:57:39 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

t_token *create_token(char *str, char *expmap)
{
	t_token *newtok;

	if (!str || !*str || !expmap || !*expmap)
		return (NULL);
	newtok = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!newtok)
		return (NULL);
	newtok->token = WORD;
	newtok->type = toktype_to_str(WORD);
	if (!newtok->type)
	{
		free(newtok);
		return (NULL);
	}
	newtok->str = ft_strdup(str);
	if (!newtok->str)
	{
		free(newtok->type);
		free(newtok);
		return (NULL);
	}
	newtok->detail.rdrc.rdrcmeta = NULL;
	newtok->detail.rdrc.rdrctarget = NULL;
	newtok->had_quote = (ft_strchr(str, '\"') || ft_strchr(str, '\''));
	newtok->expmap = ft_strdup(expmap);
	newtok->is_freed = 0;
	if (!newtok->expmap)
	{
		free(newtok->type);
		free(newtok->str);
		free(newtok);
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
		ft_printf(" ( rdrcmeta: %s rdrctarget: %s)", token->detail.rdrc.rdrcmeta, token->detail.rdrc.rdrctarget);
	}
	else if (ft_strcmp(token->type, "File") == 0)
	{
		ft_printf(" (fd: %d)", token->detail.minifile.fd);
	}
}

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

// OLD
// void	put_token_details(t_token *token)
// {
// 	if (ft_strcmp(token->type, "Pipe") == 0)
// 	{
// 		ft_printf(" (fdin: %d, fdout: %d, open_prompt: %d)",
// 			token->detail.pipe.fdin,
// 			token->detail.pipe.fdout,
// 			token->detail.pipe.open_prompt);
// 	}
// 	else if (ft_strcmp(token->type, "Redirection") == 0)
// 	{
// 		ft_printf(" (fdin: %d, fdout: %d, rdrcmeta: %s)",
// 			token->detail.rdrc.fdin,
// 			token->detail.rdrc.fdout,
// 			token->detail.rdrc.rdrcmeta ?
// 			token->detail.rdrc.rdrcmeta : "NULL");
// 	}
// 	else
// 	{
// 		ft_printf(" (arglen: %d)", token->detail.arglen);
// 	}
// }
