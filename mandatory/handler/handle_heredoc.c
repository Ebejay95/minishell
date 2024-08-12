/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:43:15 by chorst            #+#    #+#             */
/*   Updated: 2024/08/12 16:43:36 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_heredoc(t_minishell *m, t_list *current)
{
	t_token	*next_content;
	t_token	*token;
	char	*heredoc_content;
	size_t	content_size;
	size_t	content_capacity;
	char	*line;
	char	*delimiter;
	size_t	line_len;
	char	*new_content;

	token = (t_token *)current->content;
	heredoc_content = NULL;
	content_size = 0;
	content_capacity = 0;
	next_content = NULL;
	if (current->next)
		next_content = (t_token *)current->next->content;
	delimiter = next_content->str;
	while (1)
	{
		line = readline("📄 ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line_len = ft_strlen(line);
		if (content_size + line_len + 2 > content_capacity)
		{
			if (content_capacity)
				content_capacity = content_capacity * 2;
			else
				content_capacity = 1024;
			new_content = realloc(heredoc_content, content_capacity);
			if (!new_content)
			{
				free(heredoc_content);
				free(line);
			}
			heredoc_content = new_content;
		}
		ft_strcpy(heredoc_content + content_size, line);
		content_size += line_len;
		heredoc_content[content_size++] = '\n';
		heredoc_content[content_size] = '\0';
		free(line);
	}
	if (!next_content->had_quote)
		token->detail.rdrc.rdrcmeta = expand_hd(m, heredoc_content);
	else
		token->detail.rdrc.rdrcmeta = heredoc_content;
}
