/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:28:44 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/13 11:15:54 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static char	*build_segment(int start, int end, const char *prompt)
{
	int		idx;
	char	*segment;

	idx = 0;
	segment = ft_calloc((end - start) + 1, sizeof(char));
	if (!segment)
		return (NULL);
	while (idx < (end - start))
	{
		segment[idx] = prompt[start + idx];
		idx++;
	}
	segment[idx] = '\0';
	return (segment);
}

t_segment	**seg_clear_all(int idx, t_segment **segments)
{
	while (idx > 0)
	{
		idx--;
		free(segments[idx]);
	}
	free(segments);
	return (NULL);
}

t_segment	**build_segments(char const *prompt, char *type, t_segment **segments)
{
	int	i;
	int	segs_idx;
	int	start;

	i = 0;
	segs_idx = 0;
	while (prompt[i] != '\0')
	{
		while (ft_strncmp(&prompt[i], type, ft_strlen(type)) == 0 && prompt[i] != '\0')
			i += ft_strlen(type);
		start = i;
		while (ft_strncmp(&prompt[i], type, ft_strlen(type)) != 0 && prompt[i] != '\0')
			i++;
		if (start != i)
		{
			segments[segs_idx]->start = start;
			segments[segs_idx]->end = (i - 1);
			segments[segs_idx]->str = build_segment(start, i, prompt);
			if (!segments[segs_idx])
				return (seg_clear_all(segs_idx, segments));
			segs_idx++;
		}
	}
	segments[segs_idx] = NULL;
	return (segments);
}
