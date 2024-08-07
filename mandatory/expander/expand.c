/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/09 00:02:13 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	expmapcheck(char *expmap, const char *str, int i, int escaped)
{
	if (expmap[i + 1] != '\0' && str[i + 1] != '\0' && !escaped)
	{
		if (expmap[i] == '0' && expmap[i + 1] == '2' && str[i + 1] == '\"')
			return (2);
		if (expmap[i + 2] != '\0' && str[i + 2] != '\0' && expmap[i + 1] == '0' && expmap[i + 2] == '2' && str[i + 1] == '\"')
			return (2);
		else if ((expmap[i] == '0' || expmap[i] == '2') && expmap[i] == expmap[i + 1])
			return (1);
	}
	return (0);
}

void	expand(t_minishell *m, char **expanded, char **expanded_map, const char *str, char *expmap, size_t start, size_t end)
{
	char	*result;
	char	*expmap_result;
	size_t	i;
	size_t	var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;
	char	*expmap_temp;
	char	*exit_status_str;
	int		escaped;

	result = malloc(sizeof(char));
	if (!result)
		return ;
	expmap_result = malloc(sizeof(char));
	if (!expmap_result)
	{
		free(result);
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
			if (escaped == 0)
			{
				escaped = 1;
				i++;
				continue ;
			}
			else if (escaped == 1)
			{
				escaped = 0;
				temp = ft_realloc(result, ft_strlen(result) + 2);
				expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + 2);
				if (!temp || !expmap_temp)
				{
					free(result);
					free(expmap_result);
					return ;
				}
				result = temp;
				expmap_result = expmap_temp;
				ft_strncat(result, "\\", 1);
				ft_strncat(expmap_result, "0", 1);
				i++;
				continue ;
			}
		}
		if (str[i] == '$' && str[i + 1] == '?' && expmapcheck(expmap, str, i, escaped) == 1)
		{
			exit_status_str = ft_itoa(m->last_exitcode);
			if (!exit_status_str)
			{
				free(result);
				free(expmap_result);
				return ;
			}
			temp = ft_realloc(result, ft_strlen(result) + ft_strlen(exit_status_str) + 1);
			if (!temp)
			{
				free(result);
				free(expmap_result);
				free(exit_status_str);
				return ;
			}
			result = temp;
			expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + ft_strlen(exit_status_str) + 1);
			if (!expmap_temp)
			{
				free(result);
				free(expmap_result);
				free(exit_status_str);
				return ;
			}
			expmap_result = expmap_temp;
			ft_strcat(result, exit_status_str);
			ft_strfillcat(expmap_result, exit_status_str, expmap[i]);
			free(exit_status_str);
			i += 2;
		}
		else if (str[i] == '$' && expmapcheck(expmap, str, i, escaped) == 2)
		{
			i += 2;
		}
		else if (str[i] == '$' && expmapcheck(expmap, str, i, escaped) == 1)
		{
			var_start = i;
			var_name = get_var_name(str, expmap, &i);
			if (var_name)
			{
				var_value = my_getenv(var_name, m->env_list);
				if (var_value)
				{
					temp = ft_realloc(result, ft_strlen(result) + ft_strlen(var_value) + 1);
					if (!temp)
					{
						free(result);
						free(expmap_result);
						free(var_name);
						return ;
					}
					result = temp;
					ft_strcat(result, var_value);
					expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + ft_strlen(var_value) + 1);
					if (!expmap_temp)
					{
						free(result);
						free(expmap_result);
						free(var_name);
						return ;
					}
					expmap_result = expmap_temp;
					if (ft_strcontains(expmap, '2'))
					{
						ft_strfillcat(expmap_result, var_value, 'X');
					}
					else
					{
						ft_strfillcat(expmap_result, var_value, 'E');
					}
				}
				free(var_name);
			}
			else
			{
				i = var_start + 1;
				temp = ft_realloc(result, ft_strlen(result) + 2);
				if (!temp)
				{
					free(result);
					free(expmap_result);
					return ;
				}
				result = temp;
				ft_strncat(result, &str[var_start], 1);
				expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + 2);
				if (!expmap_temp)
				{
					free(result);
					free(expmap_result);
					return ;
				}
				expmap_result = expmap_temp;
				ft_strncat(expmap_result, &expmap[var_start], 1);
			}
		}
		else
		{
			if (str[i] != '"')
			{
				temp = ft_realloc(result, ft_strlen(result) + 2);
				if (!temp)
				{
					free(result);
					free(expmap_result);
					return ;
				}
				result = temp;
				ft_strncat(result, &str[i], 1);
				expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + 2);
				if (!expmap_temp)
				{
					free(result);
					free(expmap_result);
					return ;
				}
				expmap_result = expmap_temp;
				ft_strncat(expmap_result, &expmap[i], 1);
			}
			i++;
		}
	}
	*expanded = result;
	*expanded_map = expmap_result;
}
