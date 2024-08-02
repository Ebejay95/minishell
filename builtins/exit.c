/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:12:06 by chorst            #+#    #+#             */
/*   Updated: 2024/08/02 12:07:43 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	if (argv[1])
	{
		exit_status = atoi(argv[1]);
		if (exit_status == 0 && argv[1][0] != '0')
		{
			printf("exit\n🍕🚀🌈🦄🍺: exit: %s: numeric argument required\n",
				argv[1]);
			exit(2);
		}
	}
	exit(exit_status);
}

// exit status der letzten funktion oder command returnen.
