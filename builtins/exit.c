/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:12:06 by chorst            #+#    #+#             */
/*   Updated: 2024/07/03 13:27:16 by jkauker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	if (argv[1])
		exit_status = atoi(argv[1]);
	exit(exit_status);
}
