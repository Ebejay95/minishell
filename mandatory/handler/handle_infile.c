/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_infile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:46:11 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 07:24:57 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_infile(t_list *current)
{
	t_token	*next_content;
	char	*filename;
	t_token	*token;

	token = (t_token *)current->content;
	next_content = NULL;
	if (current->next)
		next_content = (t_token *)current->next->content;
	filename = ft_strdup(next_content->str);
	if (access(filename, R_OK) == -1)
	{
		ft_fprintf(2, "Error: Cannot access file '%s'\n", filename);
		return ;
	}
	if (token->rdrcmeta)
	{
		free(token->rdrcmeta);
		token->rdrcmeta = NULL;
	}
	token->rdrcmeta = filename;
}
