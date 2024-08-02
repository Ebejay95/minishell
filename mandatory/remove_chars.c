/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_chars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 09:58:55 by chorst            #+#    #+#             */
/*   Updated: 2024/08/02 11:22:20 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that removes characters from a string
// str = string to remove chars from
// chrs_to_rmv = chars to remove from str
char	*remove_chars(const char *s, const char *chrs_to_rmv)
{
	int		count;
	char	*new_str;

	count = count_relevant_chars(s, chrs_to_rmv);
	new_str = (char *)malloc(sizeof(char) * (count + 1));
	if (new_str == NULL)
		exit(EXIT_FAILURE);
	remove_helper(s, chrs_to_rmv, new_str);
	return (new_str);
}

// Function that counts the number of relevant characters in a string
int	count_relevant_chars(const char *str, const char *chrs_to_rmv)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (chrs_to_rmv[j] != '\0' && str[i] != chrs_to_rmv[j])
			j++;
		if (chrs_to_rmv[j] == '\0')
			count++;
		i++;
	}
	return (count);
}

// Function that removes characters from a string
void	remove_helper(const char *str, const char *chrs_to_rmv, char *new_s)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (chrs_to_rmv[j] != '\0' && str[i] != chrs_to_rmv[j])
			j++;
		if (chrs_to_rmv[j] == '\0')
		{
			new_s[k] = str[i];
			k++;
		}
		i++;
	}
	new_s[k] = '\0';
}
