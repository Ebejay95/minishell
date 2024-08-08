/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whitespace_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 13:51:09 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/08 17:46:49 by jeberle          ###   ########.fr       */
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
	char	*new_str;
	int		new_str_len;

	new_str_len = strlen(str) + 1;
	new_str = malloc(new_str_len);
	if (new_str == NULL)
	{
		return (NULL);
	}
	reduce_internal_whitespace(str, new_str);
	return (new_str);
}
