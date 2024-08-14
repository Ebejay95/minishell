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
	size_t	result_len;
	size_t	expmap_len;

	if (!result || !expmap_result || !escaped || !*result || !*expmap_result)
		return (-1);

	if (*escaped == 0)
	{
		*escaped = 1;
		return (1);
	}
	else if (*escaped == 1)
	{
		*escaped = 0;
		result_len = ft_strlen(*result);
		expmap_len = ft_strlen(*expmap_result);

		temp = ft_calloc(result_len + 2, sizeof(char));
		expmap_temp = ft_calloc(expmap_len + 2, sizeof(char));

		if (!temp || !expmap_temp)
		{
			free(temp);
			free(expmap_temp);
			exp_cln(result, expmap_result, NULL, NULL);
			return (-1);
		}

		ft_strlcpy(temp, *result, result_len + 1);
		ft_strlcpy(expmap_temp, *expmap_result, expmap_len + 1);

		temp[result_len] = '\\';
		expmap_temp[expmap_len] = '0';

		free(*result);
		free(*expmap_result);
		*result = temp;
		*expmap_result = expmap_temp;

		return (1);
	}
	return (0);
}
