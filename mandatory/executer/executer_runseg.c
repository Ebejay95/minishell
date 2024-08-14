/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:46:24 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 14:35:39 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	run_heredoc(t_token *t, t_fd *fd)
{
	int	hd[2];

	if (pipe(hd) == -1)
	{
		ft_fprintf(2, "Error creating pipe for heredoc\n");
		return ;
	}
	write(hd[1], t->detail.rdrc.rdrcmeta, ft_strlen(t->detail.rdrc.rdrcmeta));
	close(hd[1]);
	if (fd->last_input != fd->input)
		close(fd->last_input);
	fd->last_input = hd[0];
}

void	run_command(t_minishell *m, char **args)
{
	char	*path;

	path = get_executable(m, args[0]);
	if (path)
	{
		execute_command(m, path, args);
		free(path);
	}
	ft_fprintf(2, "bash: %s: command not found\n", args[0]);
	exit(1);
}

void	run(t_minishell *m, char **args, int arg_count, t_fd *fd)
{
	if (is_builtin(args[0]))
		exec_builtin_cmd(m, args, arg_count, fd);
	else
		execute_external_command(m, args, fd);
}

void	cleanup_fds(t_fd *fd)
{
	if (fd->last_input != fd->input)
		close(fd->last_input);
	if (fd->last_output != fd->output)
		close(fd->last_output);
}

void	run_seg(t_minishell *m, t_list *exec_lst, int input_fd, int output_fd)
{
	t_fd	fd;
	char	**args;
	int		arg_count;

	args = NULL;
	arg_count = 0;
	init_fd(&fd, input_fd, output_fd);
	process_tokens(exec_lst, &fd, &args, &arg_count);
	if (args && args[0])
		run(m, args, arg_count, &fd);
	cleanup_fds(&fd);
	ft_array_l_free(args, arg_count);
}
