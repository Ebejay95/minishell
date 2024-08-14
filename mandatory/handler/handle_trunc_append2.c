/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_trunc_append2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 15:49:42 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 15:51:23 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

t_token	*get_next_content(t_list *current)
{
	if (current->next)
		return ((t_token *)current->next->content);
	return (NULL);
}

void	handle_allocation_error(char *filecontent, char *line)
{
	ft_fprintf(2, "Error: Memory allocation failed\n");
	free(filecontent);
	free(line);
}

char	*add_line(char *cont, char *tmp, const char *line, size_t total_size)
{
	if (cont)
	{
		ft_strlcpy(tmp, cont, total_size + 1);
		free(cont);
	}
	ft_strlcpy(tmp + total_size, line, ft_strlen(line) + 1);
	return (tmp);
}
