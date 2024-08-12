/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_infile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:46:11 by chorst            #+#    #+#             */
/*   Updated: 2024/08/12 16:46:29 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_infile(t_list *current)
{
	t_token	*next_content;
	char	*filename;
	char	*line;
	char	*temp;
	size_t	total_size;
	char	*filecontent;
	t_token	*token;
	int		fd;
	size_t	line_len;

	// line_len = ft_strlen(line); // DAS HIER HAT FUER DEN SEGFAULT GESORGT
	token = (t_token *)current->content;
	total_size = 0;
	filecontent = NULL;
	next_content = NULL;
	if (current->next)
		next_content = (t_token *)current->next->content;
	filename = next_content->str;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "Error: Cannot open file '%s'\n", filename);
		return ;
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		line_len = ft_strlen(line);
		temp = ft_realloc(filecontent, total_size + line_len + 1);
		if (!temp)
		{
			ft_fprintf(2, "Error: Memory allocation failed\n");
			free(filecontent);
			free(line);
			close(fd);
			return ;
		}
		filecontent = temp;
		ft_strcpy(filecontent + total_size, line);
		total_size += line_len;
		line = get_next_line(fd);
	}
	close(fd);
	token->detail.rdrc.rdrcmeta = filename;
}
