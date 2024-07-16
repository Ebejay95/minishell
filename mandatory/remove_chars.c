/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_chars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:58:55 by chorst            #+#    #+#             */
/*   Updated: 2024/07/09 12:26:20 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that removes characters from a string
// str = string to remove chars from
// chars_to_remove = chars to remove from str
char *remove_chars(char *str, const char *chars_to_remove)
{
	int		count;
	char	*new_str;

	count = count_relevant_chars(str, chars_to_remove);
	new_str = (char *)malloc(sizeof(char) * (count + 1));
	if (new_str == NULL)
		exit(EXIT_FAILURE);
	remove_helper(str, chars_to_remove, new_str);
	return new_str;
}

// Function that counts the number of relevant characters in a string
int	count_relevant_chars(const char *str, const char *chars_to_count)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (chars_to_count[j] != '\0' && str[i] != chars_to_count[j])
			j++;
		if (chars_to_count[j] != '\0')
			count++;
		i++;
	}
	return count;
}

// Function that removes characters from a string
void remove_helper(char *str, const char *chars_to_remove, char *new_str)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (str[i] != '\0')
		{
		j = 0;
		while (chars_to_remove[j] != '\0' && str[i] != chars_to_remove[j])
			j++;
		if (chars_to_remove[j] == '\0')
		{
			new_str[k] = str[i];
			k++;
		}
		i++;
	}
	new_str[k] = '\0';
}
