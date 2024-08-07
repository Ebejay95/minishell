/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/07 14:03:14 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	expmapcheck(char *expmap, int i)
{
	if (expmap[i + 1] != '\0')
	{
		if (expmap[i] == '0' || expmap[i] == '2')
		{
			if (expmap[i] == expmap[i + 1])
			{
				return (1);
			}
		}
	}
	return (0);
}

static void	handle_exit_status(char **result, char **expmap_result, int exitcode, char *expmap, size_t *i)
{
	char *exit_status_str;
	char *temp;
	char *expmap_temp;

	exit_status_str = ft_itoa(exitcode);
	if (!exit_status_str)
	{
		free(*result);
		free(*expmap_result);
		*result = NULL;
		*expmap_result = NULL;
		return ;
	}
	temp = ft_realloc(*result, ft_strlen(*result) + ft_strlen(exit_status_str) + 1);
	expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + ft_strlen(exit_status_str) + 1);
	if (!temp || !expmap_temp)
	{
		free(*result);
		free(*expmap_result);
		free(exit_status_str);
		*result = NULL;
		*expmap_result = NULL;
		return ;
	}
	*result = temp;
	*expmap_result = expmap_temp;
	ft_strcat(*result, exit_status_str);
	ft_strfillcat(*expmap_result, exit_status_str, expmap[*i]);
	free(exit_status_str);
	*i += 2;
}

static void	handle_variable(t_minishell *m, char **result, char **expmap_result, const char *str, char *expmap, size_t *i)
{
	char *var_name = get_var_name(str, expmap, i);
	char *var_value;
	char *temp;
	char *expmap_temp;

	if (var_name)
	{
		var_value = my_getenv(var_name, m->env_list);
		if (var_value)
		{
			temp = ft_realloc(*result, ft_strlen(*result) + ft_strlen(var_value) + 1);
			expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + ft_strlen(var_value) + 1);
			if (!temp || !expmap_temp)
			{
				free(*result);
				free(*expmap_result);
				free(var_name);
				*result = NULL;
				*expmap_result = NULL;
				return ;
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
	}
	else
	{
		(*i)++;
	}
}

static void	handle_normal_char(char **result, char **expmap_result, const char *str, char *expmap, size_t *i)
{
	char *temp;
	char *expmap_temp;

	temp = ft_realloc(*result, ft_strlen(*result) + 2);
	expmap_temp = ft_realloc(*expmap_result, ft_strlen(*expmap_result) + 2);
	if (!temp || !expmap_temp)
	{
		free(*result);
		free(*expmap_result);
		*result = NULL;
		*expmap_result = NULL;
		return ;
	}
	*result = temp;
	*expmap_result = expmap_temp;
	ft_strncat(*result, &str[*i], 1);
	ft_strncat(*expmap_result, &expmap[*i], 1);
	(*i)++;
}

void	expand(t_minishell *m, char **expanded, char **expanded_map, int exitcode, const char *str, char *expmap, size_t start, size_t end)
{
	char	*result;
	char	*expmap_result;
	size_t	i;

	result = malloc(sizeof(char));
	expmap_result = malloc(sizeof(char));
	if (!result || !expmap_result)
	{
		free(result);
		free(expmap_result);
		return ;
	}
	result[0] = '\0';
	expmap_result[0] = '\0';
	i = start;
	while (i < end)
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?' && expmapcheck(expmap, i))
				handle_exit_status(&result, &expmap_result, exitcode, expmap, &i);
			else if (expmapcheck(expmap, i))
				handle_variable(m, &result, &expmap_result, str, expmap, &i);
			else
				i++;  // Skip the '$' when expmapcheck fails
		}
		else
			handle_normal_char(&result, &expmap_result, str, expmap, &i);

		if (!result || !expmap_result)
			return ;
	}
	*expanded = result;
	*expanded_map = expmap_result;
}
