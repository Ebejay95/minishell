/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whitespace_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 13:51:09 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/31 15:31:06 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	reduce_internal_whitespace(const char *str, char *new_str)
{
	int	i;
	int	j;
	int	in_whitespace;

	i = 0;
	j = 0;
	in_whitespace = 0;
	while (str[i] != '\0')
	{
		if (isspace(str[i]))
		{
			if (!in_whitespace)
			{
				new_str[j++] = ' ';
				in_whitespace = 1;
			}
		}
		else
		{
			new_str[j++] = str[i];
			in_whitespace = 0;
		}
		i++;
	}
	new_str[j] = '\0';
}

char	*whitespace_handler(const char *str)
{
	char	*trimmed_str;
	char	*new_str;
	int		new_str_len;

	trimmed_str = ft_strtrim(str, " \n\t\r\f\v");
	if (trimmed_str == NULL)
		return (NULL);
	new_str_len = strlen(trimmed_str) + 1;
	new_str = malloc(new_str_len);
	if (new_str == NULL)
	{
		free(trimmed_str);
		return (NULL);
	}
	reduce_internal_whitespace(trimmed_str, new_str);
	free(trimmed_str);
	return (new_str);
}
