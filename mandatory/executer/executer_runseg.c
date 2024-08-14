/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:46:24 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 13:09:20 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	run_heredoc(t_token *t, int *last_input_fd, int input_fd)
{
	int	hd[2];

	if (pipe(hd) == -1)
	{
		ft_fprintf(2, "Error creating pipe for heredoc\n");
		return ;
	}
	write(hd[1], t->detail.rdrc.rdrcmeta, ft_strlen(t->detail.rdrc.rdrcmeta));
	close(hd[1]);
	if (*last_input_fd != input_fd)
		close(*last_input_fd);
	*last_input_fd = hd[0];
}

void	run_in_redirection(t_token *token, int *last_input_fd, int input_fd)
{
	if (*last_input_fd != input_fd)
		close(*last_input_fd);
	*last_input_fd = open(token->detail.rdrc.rdrcmeta, O_RDONLY);
	if (*last_input_fd == -1)
	{
		ft_fprintf(2, "Error input file: %s\n", token->detail.rdrc.rdrcmeta);
		return ;
	}
}

void	run_out_redirection(t_token *token, int *last_output_fd, int output_fd)
{
	int	flags;

	if (*last_output_fd != output_fd)
		close(*last_output_fd);
	if (ft_strcmp(token->str, ">") == 0)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	*last_output_fd = open(token->detail.rdrc.rdrctarget, flags, 0644);
	if (*last_output_fd == -1)
	{
		ft_fprintf(2, "Error output file: %s\n", token->detail.rdrc.rdrctarget);
		return ;
	}
}

void	run_redirection(t_token *t, int *last_in, int *last_o, int in, int out)
{
	if (ft_strcmp(t->str, "<<") == 0)
		run_heredoc(t, last_in, in);
	else if (ft_strcmp(t->str, "<") == 0)
		run_in_redirection(t, last_in, in);
	else if (ft_strcmp(t->str, ">") == 0 || ft_strcmp(t->str, ">>") == 0)
		run_out_redirection(t, last_o, out);
}

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
		for (i = 0; i < *arg_count - 1; i++)
			temp[i] = (*args)[i];
		free(*args);
	}
	*args = temp;
	(*args)[*arg_count - 1] = arg;
	(*args)[*arg_count] = NULL;
}

void	execute_builtin_command(t_minishell *m, char **args, int arg_count, int last_input_fd, int last_output_fd, int input_fd, int output_fd)
{
	if (last_input_fd != input_fd)
		dup2(last_input_fd, STDIN_FILENO);
	if (last_output_fd != output_fd)
		dup2(last_output_fd, STDOUT_FILENO);
	execute_builtin(m, args[0], args, arg_count);
	if (ft_strcmp(args[0], "exit") == 0)
		exit(m->exitcode);
}

void	execute_external_command(t_minishell *m, char **args, int last_input_fd, int last_output_fd, int input_fd, int output_fd)
{
	pid_t	pid;
	int		status;
	char	*path;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, handle_child_process);
		signal(SIGQUIT, handle_child_process);
		if (last_input_fd != input_fd)
		{
			dup2(last_input_fd, STDIN_FILENO);
			close(last_input_fd);
		}
		if (last_output_fd != output_fd)
		{
			dup2(last_output_fd, STDOUT_FILENO);
			close(last_output_fd);
		}
		path = get_executable(m, args[0]);
		if (path)
		{
			execute_command(m, path, args);
			free(path);
		}
		ft_fprintf(2, "bash: %s: command not found\n", args[0]);
		exit(1);
	}
	else if (pid < 0)
		ft_fprintf(2, "Fork failed\n");
	else
	{
		waitpid(pid, &status, 0);
		signal(SIGINT, handle_main_process);
		signal(SIGQUIT, handle_main_process);
		if (WIFEXITED(status))
			m->exitcode = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			m->exitcode = 128 + WTERMSIG(status);
	}
}

void	run_seg(t_minishell *m, t_list *exec_lst, int input_fd, int output_fd)
{
	t_list	*current;
	t_token	*token;
	int		last_input_fd;
	int		last_output_fd;
	char	**args;
	int		arg_count;

	args = NULL;
	last_input_fd = input_fd;
	last_output_fd = output_fd;
	arg_count = 0;
	current = exec_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == REDIRECTION)
			run_redirection(token, &last_input_fd, &last_output_fd, input_fd, output_fd);
		else if (token->token == COMMAND || token->token == WORD)
			add_argument(&args, &arg_count, token->str);
		current = current->next;
	}
	if (args && args[0])
	{
		if (is_builtin(args[0]))
			execute_builtin_command(m, args, arg_count, last_input_fd, last_output_fd, input_fd, output_fd);
		else
			execute_external_command(m, args, last_input_fd, last_output_fd, input_fd, output_fd);
	}
	if (last_input_fd != input_fd)
		close(last_input_fd);
	if (last_output_fd != output_fd)
		close(last_output_fd);
	ft_array_l_free(args, arg_count);
}
