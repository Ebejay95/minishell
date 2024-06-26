/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:46:25 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/26 10:37:47 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// retrieve partials of string by several seperators to build grammar tokens
t_segment	**get_segs(char *prompt, char *type)
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
t_segment	**get_quote_segs(t_minishell *m, char type)
{
	t_segment	**segments;
	int			i;
	int			in_oc;
	int			start;
	int			end;
	size_t		seg_count;
	int			len;

	segments = NULL;
	if (!m || !m->prompt)
		return (NULL);
	i = 0;
	in_oc = 0;
	seg_count = 0;
	start = 0;
	end = 0;
	while (m->prompt[i] != '\0')
	{
		if (in_oc == 0)
		{
			if (m->prompt[i] == type)
			{
				if (i > start)
				{
					seg_count++;
				}
				in_oc = 1;
				start = i + 1;
			}
		}
		else if (in_oc == 1 && m->prompt[i] == type && (i == 0 || m->prompt[i - 1] != '\\'))
		{
			seg_count++;
			in_oc = 0;
			start = i + 1;
		}
		i++;
	}
	if ((type == '"' || type == '\'') && in_oc == 1)
		m->lexer.is_unclosed_quote = 1;
	if (start < i)
		seg_count++;
	segments = calloc(seg_count + 1, sizeof(t_segment *));
	if (segments == NULL)
		return (NULL);
	i = 0;
	seg_count = 0;
	in_oc = 0;
	start = 0;
	while (m->prompt[i] != '\0')
	{
		if (in_oc == 0)
		{
			if (m->prompt[i] == type)
			{
				if (i > start)
				{
					end = i;
					segments[seg_count] = malloc(sizeof(t_segment));
					if (!segments[seg_count])
						return (seg_clear_all(seg_count, segments));
					segments[seg_count]->start = start;
					segments[seg_count]->end = end - 1;
					len = end - start;
					segments[seg_count]->str = calloc(len + 1, sizeof(char));
					if (!segments[seg_count]->str)
						return (seg_clear_all(seg_count, segments));
					strncpy(segments[seg_count]->str, &m->prompt[start], len);
					segments[seg_count]->str[len] = '\0';
					seg_count++;
				}
				in_oc = 1;
				start = i + 1;
			}
		}
		else if (in_oc == 1 && m->prompt[i] == type && (i == 0 || m->prompt[i - 1] != '\\'))
		{
			end = i;
			segments[seg_count] = malloc(sizeof(t_segment));
			if (!segments[seg_count])
				return (seg_clear_all(seg_count, segments));
			segments[seg_count]->start = start;
			segments[seg_count]->end = end - 1;
			len = end - start;
			segments[seg_count]->str = calloc(len + 1, sizeof(char));
			if (!segments[seg_count]->str)
				return (seg_clear_all(seg_count, segments));
			strncpy(segments[seg_count]->str, &m->prompt[start], len);
			segments[seg_count]->str[len] = '\0';
			seg_count++;
			in_oc = 0;
			start = i + 1;
		}
		i++;
	}
	if (start < i)
	{
		segments[seg_count] = malloc(sizeof(t_segment));
		if (!segments[seg_count])
			return (seg_clear_all(seg_count, segments));
		segments[seg_count]->start = start;
		segments[seg_count]->end = i - 1;
		len = i - start;
		segments[seg_count]->str = calloc(len + 1, sizeof(char));
		if (!segments[seg_count]->str)
			return (seg_clear_all(seg_count, segments));
		strncpy(segments[seg_count]->str, &m->prompt[start], len);
		segments[seg_count]->str[len] = '\0';
		seg_count++;
	}
	segments[seg_count] = NULL;
	return (segments);
}

// retrieve partials of string by opening and closind quotes to build grammar tokens
//get_oc_segments(m, 'o', "AB", "67"); finde alle zeichenkettenabschnitte die entweder mit A oder B anfangen und mit '6' oder '7'enden
//get_oc_segments(m, 'a', "AB", "67"); finde alle zeichenkettenabschnitte die entweder mit "AB" anfangen und mit "67" enden
t_segment	**get_oc_segs(t_minishell *m, char mode, char *open, char *close)
{
	t_segment	**segments;
	int			i;
	int			in_oc;
	int			start;
	int			end;
	size_t		seg_count;
	int			len;
	size_t		open_len;
	size_t		close_len;

	i = 0;
	in_oc = 0;
	start = 0;
	end = 0;
	seg_count = 0;
	len = 0;
	open_len = strlen(open);
	close_len = strlen(close);
	if (!m || !m->prompt || open_len == 0 || close_len == 0)
		return (NULL);
	while (m->prompt[i] != '\0')
	{
		if (in_oc == 0)
		{
			if ((mode == 'o' && strchr(open, m->prompt[i])) || (mode == 'a' && strncmp(&m->prompt[i], open, open_len) == 0))
			{
				if (i > start)
				{
					seg_count++;
				}
				in_oc = 1;
				if (mode == 'o')
					start = 1;
				else
					start = open_len;
				if (mode == 'a')
					i += open_len - 1;
			}
		}
		else if (in_oc == 1 && strncmp(&m->prompt[i], close, close_len) == 0)
		{
			seg_count++;
			in_oc = 0;
			start = i + close_len;
			i += close_len - 1;
		}
		i++;
	}
	if (in_oc == 1)
		m->lexer.is_unclosed_quote = 1;
	if (start < i)
		seg_count++;
	segments = calloc(seg_count + 1, sizeof(t_segment *));
	if (segments == NULL)
		return (NULL);
	i = 0;
	seg_count = 0;
	in_oc = 0;
	start = 0;
	while (m->prompt[i] != '\0')
	{
		if (in_oc == 0)
		{
			if ((mode == 'o' && strchr(open, m->prompt[i])) || (mode == 'a' && strncmp(&m->prompt[i], open, open_len) == 0))
			{
				if (i > start)
				{
					end = i;
					segments[seg_count] = malloc(sizeof(t_segment));
					if (!segments[seg_count])
						return (seg_clear_all(seg_count, segments));
					segments[seg_count]->start = start;
					segments[seg_count]->end = end - 1;
					len = end - start;
					segments[seg_count]->str = calloc(len + 1, sizeof(char));
					if (!segments[seg_count]->str)
						return (seg_clear_all(seg_count, segments));
					strncpy(segments[seg_count]->str, &m->prompt[start], len);
					segments[seg_count]->str[len] = '\0';
					seg_count++;
				}
				in_oc = 1;
				if (mode == 'o')
					start = 1;
				else
					start = open_len;
				if (mode == 'a')
					i += open_len - 1;
			}
		}
		else if (in_oc == 1 && strncmp(&m->prompt[i], close, close_len) == 0)
		{
			end = i;
			segments[seg_count] = malloc(sizeof(t_segment));
			if (!segments[seg_count])
				return (seg_clear_all(seg_count, segments));
			segments[seg_count]->start = start;
			segments[seg_count]->end = end - 1;
			len = end - start;
			segments[seg_count]->str = calloc(len + 1, sizeof(char));
			if (!segments[seg_count]->str)
				return (seg_clear_all(seg_count, segments));
			strncpy(segments[seg_count]->str, &m->prompt[start], len);
			segments[seg_count]->str[len] = '\0';
			seg_count++;
			in_oc = 0;
			start = i + close_len;
			i += close_len - 1;
		}
		i++;
	}
	if (start < i)
	{
		segments[seg_count] = malloc(sizeof(t_segment));
		if (!segments[seg_count])
			return (seg_clear_all(seg_count, segments));
		segments[seg_count]->start = start;
		segments[seg_count]->end = i - 1;
		len = i - start;
		segments[seg_count]->str = calloc(len + 1, sizeof(char));
		if (!segments[seg_count]->str)
			return (seg_clear_all(seg_count, segments));
		strncpy(segments[seg_count]->str, &m->prompt[start], len);
		segments[seg_count]->str[len] = '\0';
		seg_count++;
	}
	segments[seg_count] = NULL;
	return (segments);
}

// retrieve partials of string by opening and closind quotes to build grammar tokens
//get_set_segments(m->prompt, "nids"); finde alle zeichenkettenabschnitte die aus diesen Zeichen im set bestehen (nicht alle muessen vorkommen)
t_segment	**get_set_segs(t_minishell *m, char *set)
{
	t_segment	**segments;
	int			i;
	int			start;
	int			end;
	size_t		seg_count;
	int			len;
	int			in_segment;

	segments = NULL;
	if (!m || !m->prompt || !set)
		return (NULL);
	i = 0;
	seg_count = 0;
	in_segment = 0;
	while (m->prompt[i] != '\0')
	{
		if (strchr(set, m->prompt[i]) != NULL)
		{
			if (in_segment == 0)
			{
				in_segment = 1;
				seg_count++;
			}
		}
		else
		{
			if (in_segment == 1)
			{
				in_segment = 0;
				seg_count++;
			}
			else if (i == 0 || strchr(set, m->prompt[i - 1]) != NULL)
			{
				seg_count++;
			}
		}
		i++;
	}
	segments = calloc(seg_count + 1, sizeof(t_segment *));
	if (segments == NULL)
		return (NULL);
	i = 0;
	seg_count = 0;
	start = 0;
	in_segment = 0;
	while (m->prompt[i] != '\0')
	{
		if (strchr(set, m->prompt[i]) != NULL)
		{
			if (in_segment == 0)
			{
				if (i > start)
				{
					end = i;
					segments[seg_count] = malloc(sizeof(t_segment));
					if (!segments[seg_count])
						return (seg_clear_all(seg_count, segments));
					segments[seg_count]->start = start;
					segments[seg_count]->end = end - 1;
					len = end - start;
					segments[seg_count]->str = calloc(len + 1, sizeof(char));
					if (!segments[seg_count]->str)
						return (seg_clear_all(seg_count, segments));
					strncpy(segments[seg_count]->str, m->prompt + start, len);
					segments[seg_count]->str[len] = '\0';
					seg_count++;
				}
				start = i;
				in_segment = 1;
			}
		}
		else
		{
			if (in_segment == 1)
			{
				end = i;
				segments[seg_count] = malloc(sizeof(t_segment));
				if (!segments[seg_count])
					return (seg_clear_all(seg_count, segments));
				segments[seg_count]->start = start;
				segments[seg_count]->end = end - 1;
				len = end - start;
				segments[seg_count]->str = calloc(len + 1, sizeof(char));
				if (!segments[seg_count]->str)
					return (seg_clear_all(seg_count, segments));
				strncpy(segments[seg_count]->str, m->prompt + start, len);
				segments[seg_count]->str[len] = '\0';
				seg_count++;
				start = i;
				in_segment = 0;
			}
			else if (i == 0 || strchr(set, m->prompt[i - 1]) != NULL)
			{
				start = i;
			}
		}
		i++;
	}
	if (start < i)
	{
		segments[seg_count] = malloc(sizeof(t_segment));
		if (!segments[seg_count])
			return (seg_clear_all(seg_count, segments));
		segments[seg_count]->start = start;
		segments[seg_count]->end = i - 1;
		len = i - start;
		segments[seg_count]->str = calloc(len + 1, sizeof(char));
		if (!segments[seg_count]->str)
			return (seg_clear_all(seg_count, segments));
		strncpy(segments[seg_count]->str, m->prompt + start, len);
		segments[seg_count]->str[len] = '\0';
		seg_count++;
	}
	segments[seg_count] = NULL;
	return (segments);
}
