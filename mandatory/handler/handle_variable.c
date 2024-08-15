/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:12:31 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 15:59:11 by jeberle          ###   ########.fr       */
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
	size_t	result_len;
	size_t	expmap_len;
	size_t	value_len;

	var_start = *i;
	var_name = get_var_name_exp(str, expmap, i);
	if (!var_name)
	{
		result_len = ft_strlen(*result);
		temp = ft_calloc(result_len + 2, sizeof(char));
		expmap_temp = ft_calloc(result_len + 2, sizeof(char));
		if (!temp || !expmap_temp)
		{
			free(temp);
			free(expmap_temp);
			exp_cln(result, expmap_result, &var_name, NULL);
			return (-1);
		}
		ft_strlcpy(temp, *result, result_len + 1);
		ft_strlcpy(expmap_temp, *expmap_result, result_len + 1);
		temp[result_len] = str[var_start];
		expmap_temp[result_len] = expmap[var_start];
		free(*result);
		free(*expmap_result);
		*result = temp;
		*expmap_result = expmap_temp;
		*i = var_start + 1;
		return (0);
	}
	var_value = my_getenv(var_name, m->env_list);
	if (var_value)
	{
		result_len = ft_strlen(*result);
		expmap_len = ft_strlen(*expmap_result);
		value_len = ft_strlen(var_value);
		temp = ft_calloc(result_len + value_len + 1, sizeof(char));
		expmap_temp = ft_calloc(expmap_len + value_len + 1, sizeof(char));
		if (!temp || !expmap_temp)
		{
			free(temp);
			free(expmap_temp);
			exp_cln(result, expmap_result, &var_name, NULL);
			return (-1);
		}
		ft_strlcpy(temp, *result, result_len + 1);
		ft_strlcat(temp, var_value, result_len + value_len + 1);
		ft_strlcpy(expmap_temp, *expmap_result, expmap_len + 1);
		if (ft_strcontains(expmap, '2'))
			ft_strfillcat(expmap_temp, var_value, 'X');
		else
			ft_strfillcat(expmap_temp, var_value, 'E');
		free(*result);
		free(*expmap_result);
		*result = temp;
		*expmap_result = expmap_temp;
	}
	free(var_name);
	return (0);
}
