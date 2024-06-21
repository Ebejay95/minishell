/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:56 by chorst            #+#    #+#             */
/*   Updated: 2024/06/20 10:21:06 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void ft_env(t_envlst *env_list)
{
	t_envlst *temp;

	temp = env_list;
	while (temp != NULL)
	{
		if (temp->value != NULL)
			ft_printf("%s=%s\n", temp->name, temp->value);
		else
			ft_printf("%s\n", temp->name);
		temp = temp->next;
	}
}
