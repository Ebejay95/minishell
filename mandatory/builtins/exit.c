/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:12:06 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 15:30:31 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	ft_exit(char **argv, t_minishell *m)
{
	if (argv[1])
	{
		if (m->exitcode == 0 && argv[1][0] != '0')
		{
			printf("🍕🚀🌈🦄🍺: exit: %s: %s\n",
				argv[1], NUMERIC_ARG_REQUIRED);
			exit(2);
		}
	}
	cleanup_minishell(m);
	exit(m->exitcode);
}
