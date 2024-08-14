/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:43:15 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 12:00:20 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	*read_heredoc_line(void)
{
	char	*line;

	signal(SIGINT, handle_heredoc_signal);
	line = readline("📄 ");
	signal(SIGINT, handle_main_process);
	return (line);
}

int	append_line_to_heredoc(char **heredoc_content, const char *line)
{
	char	*temp;

	temp = ft_strjoin(*heredoc_content, line);
	if (!temp)
		return (0);
	free(*heredoc_content);
	*heredoc_content = temp;
	temp = ft_strjoin(*heredoc_content, "\n");
	if (!temp)
		return (0);
	free(*heredoc_content);
	*heredoc_content = temp;
	return (1);
}

char	*read_heredoc_content(const char *delimiter)
{
	char	*heredoc_content;
	char	*line;

	heredoc_content = ft_strdup("");
	if (!heredoc_content)
		return (NULL);
	while (1)
	{
		line = read_heredoc_line();
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!append_line_to_heredoc(&heredoc_content, line))
		{
			free(line);
			free(heredoc_content);
			return (NULL);
		}
		free(line);
	}
	return (heredoc_content);
}

char	*expand_heredoc_content(t_minishell *m, char *heredoc_content)
{
	char	*expanded;

	expanded = expand_hd(m, heredoc_content);
	if (expanded)
	{
		free(heredoc_content);
		return (expanded);
	}
	return (heredoc_content);
}

void	handle_heredoc(t_minishell *m, t_list *current)
{
	t_token	*token;
	t_token	*next_content;
	char	*heredoc_content;
	char	*delimiter;

	token = (t_token *)current->content;
	next_content = get_next_content(current);
	if (next_content)
		delimiter = next_content->str;
	else
		delimiter = "";
	heredoc_content = read_heredoc_content(delimiter);
	if (!heredoc_content)
		return ;
	if (next_content && !next_content->had_quote)
		heredoc_content = expand_heredoc_content(m, heredoc_content);
	token->detail.rdrc.rdrcmeta = heredoc_content;
}
