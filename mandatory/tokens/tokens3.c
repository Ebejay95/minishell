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


void	free_token_resources(t_token *newtok)
{
	if (newtok->str)
		free(newtok->str);
	free(newtok);
}

t_token *create_token(char *str, char *expmap)
{
    t_token *newtok;

    if (!validate_input(str, expmap))
        return NULL;

    newtok = (t_token *)malloc(sizeof(t_token));
    if (!newtok)
        return NULL;

    newtok->token = WORD;
    newtok->is_freed = 0;
    newtok->str = ft_strdup(str);
    if (!newtok->str)
    {
        free(newtok);
        return NULL;
    }
    newtok->expmap = ft_strdup(expmap);
    if (!newtok->expmap)
    {
        free(newtok->str);
        free(newtok);
        return NULL;
    }
    
    newtok->rdrcmeta = NULL;
    newtok->rdrctarget = NULL;
    newtok->had_quote = (ft_strchr(str, '\"') || ft_strchr(str, '\''));
    return newtok;
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
	ft_printf("( ");
	if(token->rdrcmeta)
		ft_printf("rdrctarget: %s %p ",
		token->rdrcmeta,
		token->rdrcmeta);

	if(token->rdrctarget)
		ft_printf("rdrctarget: %s %p ",
		token->rdrctarget,
		token->rdrctarget);
	ft_printf("fd: %d ", token->fd);
	ft_printf(" )");
}
