/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_escape.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:07:00 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 12:08:08 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	allocate_new_buffers(char **t, char **et, const char *r, const char *er)
{
	size_t	result_len;
	size_t	expmap_len;

	result_len = ft_strlen(r);
	expmap_len = ft_strlen(er);
	*t = ft_calloc(result_len + 2, sizeof(char));
	*et = ft_calloc(expmap_len + 2, sizeof(char));
	if (!*t || !*et)
	{
		free(*t);
		free(*et);
		return (0);
	}
	return (1);
}

void	append_escape_character(char *t, char *e, const char *r, const char *er)
{
	size_t	result_len;
	size_t	expmap_len;

	result_len = ft_strlen(r);
	expmap_len = ft_strlen(er);
	ft_strlcpy(t, r, result_len + 1);
	ft_strlcpy(e, er, expmap_len + 1);
	t[result_len] = '\\';
	e[expmap_len] = '0';
}

int	handle_escape(char **result, char **expmap_result, int *escaped)
{
	if (!are_inputs_valid(result, expmap_result, escaped))
		return (-1);
	if (*escaped == 0)
		return (handle_first_escape(escaped));
	else if (*escaped == 1)
		return (handle_second_escape(result, expmap_result, escaped));
	return (0);
}
