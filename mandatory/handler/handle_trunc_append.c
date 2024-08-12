/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_trunc_append.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:47:13 by chorst            #+#    #+#             */
/*   Updated: 2024/08/12 16:47:28 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_trunc_append(t_list *current)
{
	t_token	*next_content;
	char	*filename;
	char	*filecontent;
	t_token	*token;
	int		fd;
	char	*line;
	char	*temp;
	size_t	total_size;
	size_t	line_len;

	filecontent = NULL;
	token = (t_token *)current->content;
	next_content = NULL;
	if (current->next)
		next_content = (t_token *)current->next->content;
	filename = next_content->str;
	if (access(filename, F_OK) != -1)
	{
		fd = open(filename, O_RDONLY);
		if (fd != -1)
		{
			total_size = 0;
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
		}
	}
	else
	{
		filecontent = ft_strdup("");
		fd = open(filename, O_WRONLY | O_CREAT, 0644);
		if (fd == -1)
		{
			ft_fprintf(2, "Error: Cannot create file '%s'\n", filename);
			free(filecontent);
			return ;
		}
		close(fd);
	}
	token->detail.rdrc.rdrcmeta = filecontent;
	token->detail.rdrc.rdrctarget = filename;
}
