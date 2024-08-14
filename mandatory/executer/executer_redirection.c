/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:29:16 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 14:31:00 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	run_in_redirection(t_token *token, t_fd *fd)
{
	if (fd->last_input != fd->input)
		close(fd->last_input);
	fd->last_input = open(token->detail.rdrc.rdrcmeta, O_RDONLY);
	if (fd->last_input == -1)
	{
		ft_fprintf(2, "Error input file: %s\n", token->detail.rdrc.rdrcmeta);
		return ;
	}
}

void	run_out_redirection(t_token *token, t_fd *fd)
{
	int	flags;

	if (fd->last_output != fd->output)
		close(fd->last_output);
	if (ft_strcmp(token->str, ">") == 0)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd->last_output = open(token->detail.rdrc.rdrctarget, flags, 0644);
	if (fd->last_output == -1)
	{
		ft_fprintf(2, "Error output file: %s\n", token->detail.rdrc.rdrctarget);
		return ;
	}
}

void	run_redirection(t_token *t, t_fd *fd)
{
	if (ft_strcmp(t->str, "<<") == 0)
		run_heredoc(t, fd);
	else if (ft_strcmp(t->str, "<") == 0)
		run_in_redirection(t, fd);
	else if (ft_strcmp(t->str, ">") == 0 || ft_strcmp(t->str, ">>") == 0)
		run_out_redirection(t, fd);
}
