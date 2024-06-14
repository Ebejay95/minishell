/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:46:25 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/13 13:34:14 by chorst           ###   ########.fr       */
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
// TODO Fix !!!!!
t_segment	**get_quote_segments(t_minishell *minishell, char type)
{
	t_segment	**segments;
	size_t		i;
	int			in_oc;
	int			start;
	int			end;
	size_t		seg_count;

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
		else if (i > 1)
		{
			if (in_oc == 1 && minishell->prompt[i] == type && minishell->prompt[i - 1] != '\\')
			{
				seg_count++;
				in_oc = 0;
			}

		}
		else if (in_oc == 1 && minishell->prompt[i] == type)
		{
			seg_count++;
			in_oc = 0;
		}
		i++;
	}
	if ((type == '"' || type == '\'') && in_oc == 1)
		minishell->lexer.is_unclosed_quote = 1;
	segments = ft_calloc(seg_count, sizeof(char *));
	if (segments == NULL)
		return (NULL);
	i = 0;
	seg_count = 0;
	in_oc = 0;
	while (minishell->prompt[i] != '\0')
	{
		if (in_oc == 0 && minishell->prompt[i] == type && minishell->prompt[i + 1])
		{
			start = i + 1;
			in_oc = 1;
		}
		else if (in_oc == 1 && minishell->prompt[i] == type && minishell->prompt[i - 1] != '\\')
		{
			end = i;
			segments[seg_count] = malloc(sizeof(t_segment));
			if (!segments[seg_count])
				return seg_clear_all(seg_count, segments);
			segments[seg_count]->start = start;
			segments[seg_count]->end = end - 1;
			segments[seg_count]->str = build_segment(start, end, minishell->prompt);
			seg_count++;
			in_oc = 0;
		}
		i++;
	}
	segments[seg_count] = NULL;
	return (segments);
}
