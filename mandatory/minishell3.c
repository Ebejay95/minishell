/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:15:04 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 07:52:53 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_envlstclear(t_minishell *minishell)
{
	t_envlst	*current;
	t_envlst	*next;

	if (minishell == NULL || minishell->env_list == NULL)
		return ;
	current = minishell->env_list;
	while (current != NULL)
	{
		next = current->next;
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	minishell->env_list = NULL;
}

void	reset_minishell_args(t_minishell *m)
{
	int	i;

	i = 0;
	while (i < MAXPIPS)
	{
		if (m->args[i] != NULL)
		{
			free(m->args[i]);
			m->args[i] = NULL;
		}
		i++;
	}
	m->argc = 0;
}

static void	i_am_freeeeeee(t_token *token)
{
	if (token != NULL)
	{
		if (token->str)
			free(token->str);
		if (token->expmap)
			free(token->expmap);
		if (token->rdrcmeta)
			free(token->rdrcmeta);
		if (token->rdrctarget)
			free(token->rdrctarget);
		token->str = NULL;
		token->expmap = NULL;
		token->rdrcmeta = NULL;
		token->rdrctarget = NULL;
		free(token);
	}
}

void	mlstclear(t_list *list)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		token = (t_token *)current->content;
		i_am_freeeeeee(token);
		if (current != NULL)
			free(current);
		current = next;
	}
}
