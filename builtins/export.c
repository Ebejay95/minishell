/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:12 by chorst            #+#    #+#             */
/*   Updated: 2024/06/14 10:45:12 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void build_export(char *key, char *value)
{
	char *tmp;

	tmp = ft_strjoin(key, "=");
	if (value)
	{
		tmp = ft_strjoin(tmp, value);
	}
	setenv(key, value, 1);
	free(tmp);
}
