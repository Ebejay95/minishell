/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:04:21 by chorst            #+#    #+#             */
/*   Updated: 2024/08/08 02:05:40 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Cleanup the minishell struct
void	cleanup_minishell(t_minishell *minishell)
{
	if (minishell->prompt)
		free(minishell->prompt);
	minishell->prompt = NULL;
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
