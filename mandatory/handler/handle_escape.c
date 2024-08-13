/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_escape.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:07:00 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 09:47:23 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	handle_escape(char **result, char **expmap_result, int *escaped)
{
	char	*temp;
	char	*expmap_temp;

	if (*escaped == 0)
	{
		*escaped = 1;
		return (1);
	}
	else if (*escaped == 1)
	{
		*escaped = 0;
		temp = ft_realloc(*result, ft_strlen(*result) + 2);
		expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + 2);
		if (!temp || !expmap_temp)
		{
			exp_cln(*result, *expmap_result, NULL, NULL);
			return (-1);
		}
		*result = temp;
		*expmap_result = expmap_temp;
		ft_strncat(*result, "\\", 1);
		ft_strncat(*expmap_result, "0", 1);
		return (1);
	}
	return (0);
}
