/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:46:24 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 00:55:38 by jonathanebe      ###   ########.fr       */
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
	write(hd[1], t->rdrcmeta, ft_strlen(t->rdrcmeta));
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

void	print_token_pointers3(t_list *tok_lst)
{
	t_list	*current;
	t_token	*c;

	ft_printf("\n"Y"AFTER RUN: Token Pointers:"D"\n");
	current = tok_lst;
	while (current != NULL)
	{
		c = (t_token *)current->content;
		ft_printf("Token address: %p\n", c);
		ft_printf("Token->str: %p (%s)\n", c->str, c->str);
		ft_printf("Token->expmap: %p (%s)\n", c->expmap, c->expmap);
		current = current->next;
	}
	ft_printf(Y"End of token pointers\n"D);
}

void	run_seg(t_minishell *m, int i, int input_fd, int output_fd)
{
	t_fd	fd;

	init_fd(&fd, input_fd, output_fd);
	if (m->pipes == 0)
		process_tok(m, m->exec_lst, &fd);
	else
		process_tok(m, m->exec_seqs[i], &fd);
	if (m->args[0])
		run(m, m->args, m->argc, &fd);
	reset_minishell_args(m);
	cleanup_fds(&fd);
}
