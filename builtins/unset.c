/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:43 by chorst            #+#    #+#             */
/*   Updated: 2024/06/18 10:51:48 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function to remove an environment variable from envp
void	ft_unset(char **envp, const char *name)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], name, ft_strlen(name)) == 0)
		{
			while (envp[i] != NULL)
			{
				envp[i] = envp[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
}
