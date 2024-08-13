/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:12:31 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 09:47:42 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	handle_variable(t_minishell *m, char **result, char **expmap_result, const char *str, char *expmap, size_t *i)
{
	size_t	var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;
	char	*expmap_temp;

	var_start = *i;
	var_name = get_var_name_exp(str, expmap, i);
	if (!var_name)
	{
		temp = ft_realloc(*result, ft_strlen(*result) + 2);
		expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + 2);
		if (!temp || !expmap_temp)
		{
			exp_cln(*result, *expmap_result, var_name, NULL);
			return (-1);
		}
		*result = temp;
		*expmap_result = expmap_temp;
		ft_strncat(*result, &str[var_start], 1);
		ft_strncat(*expmap_result, &expmap[var_start], 1);
		*i = var_start + 1;
		return (0);
	}
	var_value = my_getenv(var_name, m->env_list);
	if (var_value)
	{
		temp = ft_realloc(*result, ft_strlen(*result) + ft_strlen(var_value) + 1);
		expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + ft_strlen(var_value) + 1);
		if (!temp || !expmap_temp)
		{
			exp_cln(*result, *expmap_result, var_name, NULL);
			return (-1);
		}
		*result = temp;
		*expmap_result = expmap_temp;
		ft_strcat(*result, var_value);
		if (ft_strcontains(expmap, '2'))
			ft_strfillcat(*expmap_result, var_value, 'X');
		else
			ft_strfillcat(*expmap_result, var_value, 'E');
	}
	free(var_name);
	return (0);
}
