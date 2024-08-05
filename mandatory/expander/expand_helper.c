/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:06:01 by chorst            #+#    #+#             */
/*   Updated: 2024/08/05 18:06:16 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

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
		src_len = n;
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
	while (str[end] && ft_strchr(VS, str[end])
		&& (expmap[end] == expmap[start]))
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
