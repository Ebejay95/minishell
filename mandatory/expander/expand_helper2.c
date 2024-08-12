/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:31:40 by chorst            #+#    #+#             */
/*   Updated: 2024/08/12 16:32:15 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	append_str(char **dst, const char *src)
{
	char	*temp;

	temp = ft_realloc(*dst, ft_strlen(*dst) + ft_strlen(src) + 1);
	if (!temp)
		return (0);
	*dst = temp;
	ft_strcat(*dst, src);
	return (1);
}

int	append_char(char **dst, char c)
{
	char	*temp;
	size_t	len;

	temp = ft_realloc(*dst, ft_strlen(*dst) + 2);
	if (!temp)
		return (0);
	*dst = temp;
	len = ft_strlen(*dst);
	(*dst)[len] = c;
	(*dst)[len + 1] = '\0';
	return (1);
}
