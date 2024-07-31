/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/30 23:33:02 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

char	*get_var_name(const char *str, const char *expmap, size_t *pos)
{
	size_t	start;
	size_t	end;
	char	*var_name;

	start = *pos + 1;
	end = start;
	while (str[end] && ft_strchr(VS, str[end]) && (expmap[end] == expmap[start]))
		end++;
	if (end == start)
		return (NULL);
	var_name = malloc(end - start + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, &str[start], end - start);
	var_name[end - start] = '\0';
	*pos = end;
	return (var_name);
}

char	*expand_part(int exitcode, const char *str, char *expmap, size_t start, size_t end)
{
	char	*result;
	size_t	i;
	size_t	var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;
	char	*exit_status_str;
	size_t	j;

	result = malloc(sizeof(char));
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = start;
	while (i < end)
	{
		if (str[i] == '$' && str[i + 1] == '?' && (expmap[i] == '0' || expmap[i] == '2'))
		{
			exit_status_str = ft_itoa(exitcode);
			if (!exit_status_str)
			{
				free(result);
				return (NULL);
			}
			temp = ft_realloc(result, ft_strlen(result) + ft_strlen(exit_status_str) + 1);
			if (!temp)
			{
				free(result);
				free(exit_status_str);
				return (NULL);
			}
			result = temp;
			ft_strcat(result, exit_status_str);
			free(exit_status_str);
			i += 2;
		}
		else if (str[i] == '$' && (expmap[i] == '0' || expmap[i] == '2'))
		{
			var_start = i;
			var_name = get_var_name(str, expmap, &i);
			if (var_name)
			{
				var_value = getenv(var_name);
				if (var_value)
				{
					temp = ft_realloc(result, ft_strlen(result) + ft_strlen(var_value) + 1);
					if (!temp)
					{
						free(result);
						free(var_name);
						return (NULL);
					}
					result = temp;
					ft_strcat(result, var_value);
					j = var_start;
					while (j < i)
					{
						expmap[j] = '3';
						j++;
					}
				}
				free(var_name);
			}
			else
			{
				i = var_start + 1;
				temp = ft_realloc(result, strlen(result) + 2);
				if (!temp)
				{
					free(result);
					return (NULL);
				}
				result = temp;
				ft_strncat(result, &str[var_start], 1);
			}
		}
		else
		{
			temp = ft_realloc(result, ft_strlen(result) + 2);
			if (!temp)
			{
				free(result);
				return (NULL);
			}
			result = temp;
			ft_strncat(result, &str[i], 1);
			i++;
		}
	}
	return (result);
}

void	expand_token(int exitcode, t_token *token)
{
	char	*temp;
	size_t	len;
	char	*result;
	size_t	i;
	size_t	start;
	char	current_mode;
	char	*expanded;

	if (!token || !token->str || !token->expmap)
		return ;
	len = strlen(token->str);
	result = malloc(sizeof(char));
	if (!result)
		return ;
	result[0] = '\0';
	i = 0;
	while (i < len)
	{
		if (token->expmap[i] == '1')
		{
			start = i;
			while (i < len && token->expmap[i] == '1')
				i++;
			temp = realloc(result, strlen(result) + (i - start) + 1);
			if (!temp)
			{
				free(result);
				return ;
			}
			result = temp;
			strncat(result, &token->str[start], i - start);
		}
		else
		{
			start = i;
			current_mode = token->expmap[i];
			while (i < len && token->expmap[i] == current_mode)
				i++;
			expanded = expand_part(exitcode, &token->str[start], &token->expmap[start], 0, i - start);
			if (expanded)
			{
				temp = ft_realloc(result, ft_strlen(result) + ft_strlen(expanded) + 1);
				if (!temp)
				{
					free(result);
					free(expanded);
					return ;
				}
				result = temp;
				ft_strcat(result, expanded);
				free(expanded);
			}
		}
	}
	free(token->str);
	token->str = result;
}
