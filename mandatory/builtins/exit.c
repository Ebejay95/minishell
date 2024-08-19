/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:12:06 by chorst            #+#    #+#             */
/*   Updated: 2024/08/19 10:09:14 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(char **argv, t_minishell *m)
{
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			printf("🍕🚀🌈🦄🍺: exit: %s: %s\n", argv[1], NUMERIC_ARG_REQUIRED);
			exit(2);
		}
		exit(ft_atoi(argv[1]));
	}
	cleanup_minishell(m, 0);
	exit(m->exitcode);
}
