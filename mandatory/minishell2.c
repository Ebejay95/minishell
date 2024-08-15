/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:04:21 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 19:26:14 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static void ft_envlstclear(t_minishell *minishell)
{
	t_envlst    *current;
	t_envlst    *next;

	if (minishell == NULL || minishell->env_list == NULL)
		return;
	current = minishell->env_list;
	while (current != NULL)
	{
		next = current->next;
		if (current->name)
		{
			free(current->name);
			current->name = NULL;
		}
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		current = next;
	}
	minishell->env_list = NULL;
}

// Cleanup the minishell struct
void	cleanup_minishell(t_minishell *minishell)
{
	if (minishell->prompt && global == 0)
		free(minishell->prompt);
	minishell->prompt = NULL;
	ft_envlstclear(minishell);
}

// Initialize the minishell struct
void	initialize_minishell(t_minishell *minishell, char **envp)
{
	minishell->leave = 0;
	minishell->last_exitcode = 0;
	minishell->exitcode = 0;
	minishell->envp = envp;
	minishell->prompt = NULL;
}
