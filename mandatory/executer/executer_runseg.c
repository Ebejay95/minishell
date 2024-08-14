/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:46:24 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 13:46:10 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	add_argument(char ***args, int *arg_count, char *arg)
{
	char	**temp;
	int		i;

	(*arg_count)++;
	temp = ft_calloc(*arg_count + 1, sizeof(char *));
	if (!temp)
	{
		ft_fprintf(2, "Error: Memory allocation failed\n");
		ft_array_l_free(*args, *arg_count - 1);
		return ;
	}
	if (*args)
	{
		i = 0;
		while (i < *arg_count - 1)
		{
			temp[i] = (*args)[i];
			i++;
		}
		free(*args);
	}
	*args = temp;
	(*args)[*arg_count - 1] = arg;
	(*args)[*arg_count] = NULL;
}

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

void	exec_builtin_cmd(t_minishell *m, char **args, int arg_count, t_fd *fd)
{
	if (fd->last_input != fd->input)
		dup2(fd->last_input, STDIN_FILENO);
	if (fd->last_output != fd->output)
		dup2(fd->last_output, STDOUT_FILENO);
	execute_builtin(m, args[0], args, arg_count);
	if (ft_strcmp(args[0], "exit") == 0)
		exit(m->exitcode);
}

void	setup_child_process(t_fd *fd)
{
	signal(SIGINT, handle_child_process);
	signal(SIGQUIT, handle_child_process);
	if (fd->last_input != fd->input)
	{
		dup2(fd->last_input, STDIN_FILENO);
		close(fd->last_input);
	}
	if (fd->last_output != fd->output)
	{
		dup2(fd->last_output, STDOUT_FILENO);
		close(fd->last_output);
	}
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

void	run_parent_process(t_minishell *m, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	signal(SIGINT, handle_main_process);
	signal(SIGQUIT, handle_main_process);
	if (WIFEXITED(status))
		m->exitcode = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		m->exitcode = 128 + WTERMSIG(status);
}

void	execute_external_command(t_minishell *m, char **args, t_fd *fd)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		setup_child_process(fd);
		run_command(m, args);
	}
	else if (pid < 0)
		ft_fprintf(2, "Fork failed\n");
	else
		run_parent_process(m, pid);
}

void	init_fd(t_fd *fd, int input_fd, int output_fd)
{
	fd->input = input_fd;
	fd->output = output_fd;
	fd->last_input = input_fd;
	fd->last_output = output_fd;
}

void	process_tokens(t_list *exec_lst, t_fd *fd, char ***args, int *arg_count)
{
	t_list	*current;
	t_token	*token;

	current = exec_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == REDIRECTION)
			run_redirection(token, fd);
		else if (token->token == COMMAND || token->token == WORD)
			add_argument(args, arg_count, token->str);
		current = current->next;
	}
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
