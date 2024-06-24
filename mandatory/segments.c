/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:46:25 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/24 07:43:30 by jkauker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// retrieve partials of string by several seperators to build grammar tokens
t_segment	**get_segments(char *prompt, char *type)
{
	t_segment		**segments;
	// unsigned char	ctype;
	size_t			seg_count;
	size_t			i;

	if (!prompt)
		return (NULL);
	// ctype = (unsigned char)type;
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

// retrieve partials of string by opening and closind quotes to build grammar tokens
t_segment	**get_quote_segments(t_minishell *minishell, char type)
{
	t_segment	**segments;
	size_t		i;
	int			in_oc;
	int			start;
	int			end;
	int			idx;
	size_t		seg_count;
	int			len;

	segments = NULL;
	if (!minishell || !minishell->prompt)
		return (NULL);
	i = 0;
	in_oc = 0;
	seg_count = 0;
	start = 0;
	end = 0;
	while (minishell->prompt[i] != '\0')
	{
		if (in_oc == 0 && minishell->prompt[i] == type)
			in_oc = 1;
		else if (in_oc == 1 && minishell->prompt[i] == type && (i == 0 || minishell->prompt[i - 1] != '\\'))
		{
			seg_count++;
			in_oc = 0;
		}
		i++;
	}
	if ((type == '"' || type == '\'') && in_oc == 1)
		minishell->lexer.is_unclosed_quote = 1;
	segments = ft_calloc(seg_count + 1, sizeof(t_segment *));
	if (segments == NULL)
		return (NULL);
	i = 0;
	seg_count = 0;
	in_oc = 0;
	while (minishell->prompt[i] != '\0')
	{
		if (in_oc == 0 && minishell->prompt[i] == type)
		{
			start = i + 1;
			in_oc = 1;
		}
		else if (in_oc == 1 && minishell->prompt[i] == type && (i == 0 || minishell->prompt[i - 1] != '\\'))
		{
			end = i;
			segments[seg_count] = malloc(sizeof(t_segment));
			if (!segments[seg_count])
				return (seg_clear_all(seg_count, segments));
			segments[seg_count]->start = start;
			segments[seg_count]->end = end - 1;
			idx = 0;
			len = end - start;
			segments[seg_count]->str = ft_calloc(len + 1, sizeof(char));
			if (!segments[seg_count]->str)
				return (seg_clear_all(seg_count, segments));
			while (idx < len)
			{
				segments[seg_count]->str[idx] = minishell->prompt[start + idx];
				idx++;
			}
			segments[seg_count]->str[idx] = '\0';
			if (ft_strlen(segments[seg_count]->str) == 0)
				segments[seg_count]->start = segments[seg_count]->end;
			seg_count++;
			in_oc = 0;
		}
		i++;
	}
	segments[seg_count] = NULL;
	return (segments);
}
