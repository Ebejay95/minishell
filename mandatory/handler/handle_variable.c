/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:12:31 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 15:49:24 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	handle_no_variable(t_var_data *data)
{
	size_t	result_len;
	char	*temp;
	char	*expmap_temp;

	result_len = ft_strlen(*(data->result));
	*temp = ft_calloc(result_len + 2, sizeof(char));
	*expmap_temp = ft_calloc(result_len + 2, sizeof(char));
	if (!temp || !expmap_temp)
	{
		free(temp);
		free(expmap_temp);
		exp_cln(data->result, data->expmap_result, &(data->var_name), NULL);
		return (-1);
	}
	ft_strlcpy(temp, *(data->result), result_len + 1);
	ft_strlcpy(expmap_temp, *(data->expmap_result), result_len + 1);
	temp[result_len] = data->str[data->var_start];
	expmap_temp[result_len] = data->expmap[data->var_start];
	free(*(data->result));
	free(*(data->expmap_result));
	*(data->result) = temp;
	*(data->expmap_result) = expmap_temp;
	*(data->i) = data->var_start + 1;
	return (0);
}

int	append_var_value(t_var_data *data)
{
	size_t	result_len;
	size_t	expmap_len;
	size_t	value_len;
	char	*temp;
	char	*expmap_temp;

	result_len = ft_strlen(*(data->result));
	expmap_len = ft_strlen(*(data->expmap_result));
	value_len = ft_strlen(data->var_value);
	temp = ft_calloc(result_len + value_len + 1, sizeof(char));
	expmap_temp = ft_calloc(expmap_len + value_len + 1, sizeof(char));
	if (!temp || !expmap_temp)
	{
		free(temp);
		free(expmap_temp);
		exp_cln(data->result, data->expmap_result, &(data->var_name), NULL);
		return (-1);
	}
	ft_strlcpy(temp, *(data->result), result_len + 1);
	ft_strlcat(temp, data->var_value, result_len + value_len + 1);
	ft_strlcpy(expmap_temp, *(data->expmap_result), expmap_len + 1);
	if (ft_strcontains(data->expmap, '2'))
		ft_strfillcat(expmap_temp, data->var_value, 'X');
	else
		ft_strfillcat(expmap_temp, data->var_value, 'E');
	free(*(data->result));
	free(*(data->expmap_result));
	*(data->result) = temp;
	*(data->expmap_result) = expmap_temp;
	return (0);
}

int	handle_variable(t_minishell *m, char **result, char **expmap_result, const char *str, char *expmap, size_t *i)
{
	t_var_data	data = {result, expmap_result, str, expmap, i, *i, NULL, NULL};

	data.var_name = get_var_name_exp(str, expmap, i);
	if (!data.var_name)
	{
		return (handle_no_variable(&data));
	}
	data.var_value = my_getenv(data.var_name, m->env_list);
	if (data.var_value)
	{
		if (append_var_value(&data) == -1)
			return (-1);
	}
	free(data.var_name);
	return (0);
}
