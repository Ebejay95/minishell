/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:46:25 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/13 12:09:05 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

t_segment	**get_segments(char *prompt, char *type)
{
	t_segment		**segments;
	unsigned char	ctype;
	size_t			seg_count;
	size_t			i;

	if (!prompt)
		return (NULL);
	ctype = (unsigned char)type;
	seg_count = ft_count_words_b_str(prompt, type);
	if (seg_count <= 1)
		return (NULL);
	segments = ft_calloc(seg_count + 1, sizeof(char *));
	if (segments == NULL)
		return (NULL);
	i = 0;
	while (i < seg_count)
	{
		segments[i] = malloc(sizeof(t_segment));
		if (!segments[i])
			return (seg_clear_all(i, segments));
		i++;
	}
	return (build_segments(prompt, type, segments));
}

t_segment	**get_oc_segments(char *prompt, char type)
{
	t_segment	**segments;
	size_t		i;
	int			in_oc;
	size_t		seg_count;

	if (!prompt)
		return (NULL);
	i = 0;
	in_oc = 0;
	seg_count = 0;
	while (prompt[i] != '\0')
	{
		if (in_oc == 0)
		{
			in_oc = 0;
		}
		i++;
	}
	if (type == '"' || '\'' && in_oc == 1)
		lexer
	return (segments);
}
