/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/12 16:21:35 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	expmapcheck(char *expmap, const char *str, int i, int escaped)
{
	if (expmap[i + 1] != '\0' && str[i + 1] != '\0' && !escaped)
	{
		if (expmap[i] == '0'
			&& expmap[i + 1] == '2'
			&& str[i + 1] == '\"')
			return (2);
		if (expmap[i + 2] != '\0'
			&& str[i + 2] != '\0'
			&& expmap[i + 1] == '0'
			&& expmap[i + 2] == '2'
			&& str[i + 1] == '\"')
			return (2);
		else if ((expmap[i] == '0' || expmap[i] == '2')
			&& expmap[i] == expmap[i + 1])
			return (1);
	}
	return (0);
}

void	exp_cleanup(char *result, char *expmap_result, char *var_name, char *exit_status_str)
{
	free(result);
	free(expmap_result);
	free(var_name);
	free(exit_status_str);
}

static int	handle_escape(char **result, char **expmap_result, int *escaped)
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
			exp_cleanup(*result, *expmap_result, NULL, NULL);
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

static int	handle_exit_status(t_minishell *m, char **result, char **expmap_result)
{
	char	*exit_status_str;
	char	*temp;
	char	*expmap_temp;

	exit_status_str = ft_itoa(m->last_exitcode);
	if (!exit_status_str)
	{
		exp_cleanup(*result, *expmap_result, NULL, NULL);
		return (-1);
	}
	temp = ft_realloc(*result, ft_strlen(*result) + ft_strlen(exit_status_str) + 1);
	expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + ft_strlen(exit_status_str) + 1);
	if (!temp || !expmap_temp)
	{
		exp_cleanup(*result, *expmap_result, NULL, exit_status_str);
		return (-1);
	}
	*result = temp;
	*expmap_result = expmap_temp;
	ft_strcat(*result, exit_status_str);
	ft_strfillcat(*expmap_result, exit_status_str, (*expmap_result)[ft_strlen(*expmap_result) - 1]);
	free(exit_status_str);
	return (2);
}

static int	handle_variable(t_minishell *m, char **result, char **expmap_result, const char *str, char *expmap, size_t *i)
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
			exp_cleanup(*result, *expmap_result, var_name, NULL);
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
			exp_cleanup(*result, *expmap_result, var_name, NULL);
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

void	expand(t_minishell *m, char **expanded, char **expanded_map, const char *str, char *expmap, size_t start, size_t end)
{
	char	*result;
	char	*expmap_result;
	size_t	i;
	int		escaped;
	int		ret;

	result = malloc(sizeof(char));
	expmap_result = malloc(sizeof(char));
	if (!result || !expmap_result)
	{
		exp_cleanup(result, expmap_result, NULL, NULL);
		*expanded = NULL;
		*expanded_map = NULL;
		return ;
	}
	result[0] = '\0';
	expmap_result[0] = '\0';
	i = start;
	escaped = 0;
	while (i < end)
	{
		if (expmap[i] == 'S')
		{
			i++;
			continue ;
		}
		if (str[i] == '\\')
		{
			ret = handle_escape(&result, &expmap_result, &escaped);
			if (ret == -1)
			{
				*expanded = NULL;
				*expanded_map = NULL;
				return ;
			}
			if (ret > 0)
			{
				i += ret;
				continue ;
			}
		}
		if (str[i] == '$' && str[i + 1] == '?' && expmapcheck(expmap, str, i, escaped) == 1)
		{
			ret = handle_exit_status(m, &result, &expmap_result);
			if (ret == -1)
			{
				*expanded = NULL;
				*expanded_map = NULL;
				return ;
			}
			i += ret;
		}
		else if (str[i] == '$' && expmapcheck(expmap, str, i, escaped) == 2)
			i += 2;
		else if (str[i] == '$' && expmapcheck(expmap, str, i, escaped) == 1)
		{
			ret = handle_variable(m, &result, &expmap_result, str, expmap, &i);
			if (ret == -1)
			{
				*expanded = NULL;
				*expanded_map = NULL;
				return ;
			}
		}
		else
		{
			if (str[i] != '"')
			{
				char *temp = ft_realloc(result, ft_strlen(result) + 2);
				char *expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + 2);
				if (!temp || !expmap_temp)
				{
					exp_cleanup(result, expmap_result, NULL, NULL);
					*expanded = NULL;
					*expanded_map = NULL;
					return ;
				}
				result = temp;
				expmap_result = expmap_temp;
				ft_strncat(result, &str[i], 1);
				ft_strncat(expmap_result, &expmap[i], 1);
			}
			i++;
		}
	}
	*expanded = result;
	*expanded_map = expmap_result;
}
