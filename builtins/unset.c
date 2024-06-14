/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:43 by chorst            #+#    #+#             */
/*   Updated: 2024/06/14 12:00:20 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	builtin_unset(char **envp, char **args)
{
	(void)envp;
	if (args[1] == NULL)
	{
		ft_printf("unset: not enough arguments\n");
		return ;
	}
	unset(args);
}
