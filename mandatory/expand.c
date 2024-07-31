/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/31 16:32:06 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_strfillcat(char *dest, const char *src, char fill_char)
{
	char	*dest_end;
	size_t	src_len;
	size_t	i;

	dest_end = dest + ft_strlen(dest);
	src_len = ft_strlen(src);
	i = 0;
	while (i < src_len)
	{
		dest_end[i] = fill_char;
		i++;
	}
	dest_end[src_len] = '\0';
}

void	ft_strfillncat(char *dest, const char *src, size_t n, char fill_char)
{
	char	*dest_end;
	size_t	src_len;
	size_t	i;

	dest_end = dest + strlen(dest);
	src_len = strlen(src);
	i = 0;
	if (src_len > n)
	{
		src_len = n;
	}
	while (i < src_len)
	{
		dest_end[i] = fill_char;
		i++;
	}
	dest_end[src_len] = '\0';
}

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

void	expand_part(char **expanded, char **expanded_map, int exitcode, const char *str, char *expmap, size_t start, size_t end)
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
	while (i < end)
	{
		if (str[i] == '$' && str[i + 1] == '?' && (expmap[i] == '0' || expmap[i] == '2'))
		{
			exit_status_str = ft_itoa(exitcode);
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
				temp = ft_realloc(result, strlen(result) + 2);
				if (!temp)
				{
					free(result);
					free(expmap_result);
					return ;
				}
				result = temp;
				ft_strncat(result, &str[var_start], 1);
				expmap_temp = ft_realloc(expmap_result, strlen(expmap_result) + 2);
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
			i++;
		}
	}
	*expanded = result;
	*expanded_map = expmap_result;
}

void	expand_token(int exitcode, t_token *token)
{
	char	*temp;
	char	*expmap_temp;
	size_t	len;
	size_t	i;
	size_t	start;
	char	current_mode;
	char	*expanded;
	char	*expanded_map;
	char	*result;
	char	*expmap_result;

	expanded = NULL;
	expanded_map = NULL;
	if (!token || !token->str || !token->expmap)
		return ;
	len = strlen(token->str);
	result = malloc(sizeof(char));
	if (!result)
		return ;
	result[0] = '\0';
	expmap_result = malloc(sizeof(char));
	if (!expmap_result)
		return ;
	expmap_result[0] = '\0';
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
			expand_part(&expanded, &expanded_map, exitcode, &token->str[start], &token->expmap[start], 0, i - start);
			if (expanded && expanded_map)
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
				expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + ft_strlen(expanded_map) + 1);
				if (!expmap_temp)
				{
					free(expmap_result);
					free(expanded_map);
					return ;
				}
				expmap_result = expmap_temp;
				ft_strcat(expmap_result, expanded_map);
			}
		}
	}
	free(token->str);
	token->str = result;
	token->expmap = expmap_result;
}
