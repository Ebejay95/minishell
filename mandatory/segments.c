/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:46:25 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/17 13:52:25 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

int	is_redirection_pattern(const char *str, int i)
{
	if (str[i] == '>' || str[i] == '<')
	{
		while (str[i] == '>' || str[i] == '<')
			i++;
        if (str[i + 1])
        {
        	if (str[i + 1] == '\n')
        	{
        	    fprintf(stderr, "bash: syntax error near unexpected token `\\\n'\n");
        	    return 0;
        	}
        	if (str[i + 1] == '|')
        	{
        	    fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
        	    return 0;
        	}
        }
		return (i);
	}
	
	return (0);
}

int is_pipe_pattern(const char *str, int i)
{
    if (str[i] == '|')
    {
        i++;

        if (str[i] == '|')
        {
            fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
            return 0;
        }

        if (str[i] == '\0')
        {
            return i;
        }
        return i;
    }
    return 0;
}

int is_ges_pattern(const char *str, int i)
{
    if (str[i] == '$')
    {
        if (str[i + 1] == '\0' || (!ft_strchr(VS, str[i + 1]) && str[i + 1] != '?'))
        {
            fprintf(stderr, "bash: syntax error near unexpected token `$'\n");
            return (0);
        }
        if (str[i + 1] == '?')
        {
            return (i + 2);
        }
    }
    return (0);
}

int is_var_pattern(const char *str, int i)
{
    if (str[i] == '$')
    {
        if (str[i + 1] == '\0' || (!ft_strchr(VS, str[i + 1]) && str[i + 1] != '?'))
        {
            fprintf(stderr, "bash: syntax error near unexpected token `$'\n");
            return (0);
        }
        i++;
        while (str[i] && (ft_strchr(VS, str[i]) || str[i] == '?'))
        {
            if (str[i] == '$')
            {
                fprintf(stderr, "bash: syntax error near unexpected token `$'\n");
                return (0);
            }
            i++;
        }
        return (i);
    }
    return (0);
}

t_segment	*create_segment(const char *str, int start, int end, t_toktype type)
{
	t_segment	*segment;

	segment = malloc(sizeof(t_segment));
	if (!segment)
		return (NULL);
	segment->str = strndup(str + start, end - start);
	segment->type = type;
	return (segment);
}

t_segment	**lex(t_token *token, t_toktype lookfor)
{
	t_segment	**segments;
	int			count;
	int			i;
	int			last_pos;
	int			new_pos;

	segments = NULL;
	count = 0;
	i = 0;
	last_pos = 0;
	new_pos = 0;
	while (token->str[i])
	{
		if (lookfor == REDIRECTION)
			new_pos = is_redirection_pattern(token->str, i);
		else if (lookfor == PIPE)
			new_pos = is_pipe_pattern(token->str, i);
		else if (lookfor == GETEXSTATE)
			new_pos = is_ges_pattern(token->str, i);
		else if (lookfor == VARIABLE)
			new_pos = is_var_pattern(token->str, i);
		else
			new_pos = 0;

		if (new_pos)
		{
			if (i > last_pos)
			{
				segments = realloc(segments, sizeof(t_segment *) * (count + 1));
				segments[count++] = create_segment(token->str, last_pos, i, token->token);
			}
			if (new_pos > i)
			{
				segments = realloc(segments, sizeof(t_segment *) * (count + 1));
				segments[count++] = create_segment(token->str, i, new_pos, lookfor);
			}
			last_pos = new_pos;
			i = new_pos;
		}
		else
		{
			i++;
		}
	}
	if (last_pos < i)
	{
		segments = realloc(segments, sizeof(t_segment *) * (count + 1));
		segments[count++] = create_segment(token->str, last_pos, i, token->token);
	}
	segments = realloc(segments, sizeof(t_segment *) * (count + 1));
	segments[count] = NULL;
	return (segments);
}
