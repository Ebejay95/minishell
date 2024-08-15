/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_escape2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:07:00 by chorst            #+#    #+#             */
/*   Updated: 2024/08/16 00:03:33 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	are_inputs_valid(char **result, char **expmap_result, int *escaped)
{
	return (result && expmap_result && escaped && *result && *expmap_result);
}

int	handle_first_esc(int *escaped)
{
	*escaped = 1;
	return (1);
}

int	handle_second_esc(char **result, char **expmap_result, int *escaped)
{
	char	*temp;
	char	*expmap_temp;

	*escaped = 0;
	if (!allocate_new_buf(&temp, &expmap_temp, *result, *expmap_result))
	{
		expcn(result, expmap_result, NULL, NULL);
		return (-1);
	}
	append_esc_character(temp, expmap_temp, *result, *expmap_result);
	free(*result);
	free(*expmap_result);
	*result = temp;
	*expmap_result = expmap_temp;
	return (1);
}
