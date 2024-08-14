/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:04:21 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 23:25:15 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static void	ft_envlstclear(t_minishell *minishell)
{
	t_envlst	*buffer;

	if (minishell->env_list == NULL)
		return ;
	while (minishell->env_list != NULL)
	{
		buffer = (minishell->env_list)->next;
		if (minishell->env_list->name)
			free(minishell->env_list->name);
		if (minishell->env_list->value)
			free(minishell->env_list->value);
		minishell->env_list = buffer;
	}
}

// Cleanup the minishell struct
void	cleanup_minishell(t_minishell *minishell)
{
	ft_printf("free_tcleanup_minishelloken\n");
	if (minishell->prompt)
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
