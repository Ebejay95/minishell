/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:56 by chorst            #+#    #+#             */
/*   Updated: 2024/06/14 09:28:31 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	builtin_env(char **env)
{
	int		i;
	char	*print;

	i = 0;
	while (env[i] != 0)
	{
		print = ft_color(env[i], YELLOW);
		ft_printf(print);
		ft_printf("\n");
		free(print);
		i++;
	}
}
