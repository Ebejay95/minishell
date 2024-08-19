/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_trunc_append.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:47:13 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 15:52:38 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	*read_file_content(int fd)
{
	t_temps	t;
	size_t	total_size;
	size_t	line_len;

	t.filecontent = NULL;
	total_size = 0;
	while (1)
	{
		t.line = get_next_line(fd);
		if (t.line == NULL)
			break ;
		line_len = ft_strlen(t.line);
		t.temp = ft_calloc(total_size + line_len + 1, sizeof(char));
		if (!t.temp)
			return (handle_allocation_error(t.filecontent, t.line), NULL);
		t.filecontent = add_line(t.filecontent, t.temp, t.line, total_size);
		total_size += line_len;
		free(t.line);
	}
	if (t.filecontent)
		return (t.filecontent);
	else
		return (ft_strdup(""));
}

char	*read_existing_file(const char *filename)
{
	int		fd;
	char	*filecontent;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (ft_strdup(""));
	filecontent = read_file_content(fd);
	close(fd);
	return (filecontent);
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
		return (NULL);
	}
	close(fd);
	return (filecontent);
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
	if(token->rdrcmeta)
	{
		free(token->rdrcmeta);
		token->rdrcmeta = NULL;
	}
	token->rdrcmeta = filecontent;
	token->rdrctarget = ft_strdup(filename);
}
