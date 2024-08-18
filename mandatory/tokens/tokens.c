/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:12:44 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 10:20:52 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	validate_input(const char *str, const char *expmap)
{
	return (str && *str && expmap && *expmap);
}

t_token	*allocate_token(void)
{
	t_token	*newtok;

	newtok = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!newtok)
		return (NULL);
	newtok->token = WORD;
	newtok->is_freed = 0;
	return (newtok);
}

int	set_token_str(t_token *newtok, char *str)
{
	newtok->str = ft_strdup(str);
	if (!newtok->str)
		return (0);
	return (1);
}

void	set_token_details(t_token *newtok, const char *str)
{
	newtok->rdrcmeta = NULL;
	newtok->rdrctarget = NULL;
	newtok->had_quote = (ft_strchr(str, '\"') || ft_strchr(str, '\''));
}
