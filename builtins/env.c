/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:56 by chorst            #+#    #+#             */
/*   Updated: 2024/06/24 13:26:36 by jkauker          ###   ########.fr       */
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
		temp = temp->next;
	}
}
