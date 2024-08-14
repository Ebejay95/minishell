/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_trunc_append.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:47:13 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 11:55:16 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

t_token	*get_next_content(t_list *current)
{
	if (current->next)
		return (t_token *)current->next->content;
	return NULL;
}

void	handle_allocation_error(char *filecontent, char *line)
{
	ft_fprintf(2, "Error: Memory allocation failed\n");
	free(filecontent);
	free(line);
}

char	*append_line_to_content(char *filecontent, char *temp, const char *line, size_t total_size)
{
	if (filecontent)
	{
		ft_strlcpy(temp, filecontent, total_size + 1);
		free(filecontent);
	}
	ft_strlcpy(temp + total_size, line, ft_strlen(line) + 1);
	return temp;
}

char	*read_file_content(int fd)
{
	char	*filecontent;
	char	*line;
	char	*temp;
	size_t	total_size;
	size_t	line_len;

	filecontent = NULL;
	total_size = 0;

	while ((line = get_next_line(fd)) != NULL)
	{
		line_len = ft_strlen(line);
		temp = ft_calloc(total_size + line_len + 1, sizeof(char));
		if (!temp)
		{
			handle_allocation_error(filecontent, line);
			return NULL;
		}
		filecontent = append_line_to_content(filecontent, temp, line, total_size);
		total_size += line_len;
		free(line);
	}

	return filecontent ? filecontent : ft_strdup("");
}

char	*read_existing_file(const char *filename)
{
	int		fd;
	char	*filecontent;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return ft_strdup("");

	filecontent = read_file_content(fd);
	close(fd);

	return filecontent;
}

char	*create_new_file(const char *filename)
{
	int		fd;
	char	*filecontent;

	filecontent = ft_strdup("");
	fd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_fprintf(2, "Error: Cannot create file '%s'\n", filename);
		free(filecontent);
		return NULL;
	}
	close(fd);

	return filecontent;
}

void	handle_trunc_append(t_list *current)
{
	t_token	*token;
	t_token	*next_content;
	char	*filename;
	char	*filecontent;

	token = (t_token *)current->content;
	next_content = get_next_content(current);
	filename = next_content->str;

	if (access(filename, F_OK) != -1)
		filecontent = read_existing_file(filename);
	else
		filecontent = create_new_file(filename);

	if (!filecontent)
		return ;

	token->detail.rdrc.rdrcmeta = filecontent;
	token->detail.rdrc.rdrctarget = filename;
}

