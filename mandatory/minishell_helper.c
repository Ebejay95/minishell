/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:04:21 by chorst            #+#    #+#             */
/*   Updated: 2024/07/23 14:46:14 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Cleanup the minishell struct
void cleanup_minishell(t_minishell *minishell)
{
	free(minishell->prompt);
	// minishell->prompt = NULL;
}

// Initialize the minishell struct
void initialize_minishell(t_minishell *minishell, char **envp)
{
	minishell->envp = envp;
	minishell->prompt = NULL;
}

// Function that defines the interactive mode
void	interactive_mode(t_minishell *minishell)
{
	minishell->prompt = readline("🍕🚀🌈🦄🍺 ");
	if (!minishell->prompt)
		return ((void)printf("\033[1A🍕🚀🌈🦄🍺 exit\n"));
}

// Function that defines the non-interactive mode
void	non_interactive_mode(t_minishell *minishell)
{
	char buffer[1024];
	ssize_t bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, 1024);
	if (bytes_read <= 0)
		return ;
	buffer[bytes_read] = '\0';
	minishell->prompt = strdup(buffer);
	if (!minishell->prompt)
		return (perror("strdup"));
}
