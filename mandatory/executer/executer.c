/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 11:48:42 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	prexecute(t_minishell *m, t_list **tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;

	current = *tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (!ft_strcmp(token->str, "<<"))
		{
			handle_heredoc(m, current);
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		else if (!ft_strcmp(token->str, "<"))
		{
			handle_infile(current);
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		current = current->next;
	}
	current = *tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (keep_for_exec(token))
		{
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		current = current->next;
	}
	current = *tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (!ft_strcmp(token->str, ">") || !ft_strcmp(token->str, ">>"))
		{
			handle_trunc_append(current);
			ft_lstadd_back(exec_lst, ft_lstnew(current->content));
		}
		current = current->next;
	}
}

void	run_seg(t_minishell *m, t_list *exec_lst, int input_fd, int output_fd)
{
	t_list	*current;
	t_token	*token;
	int		last_input_fd;
	int		last_output_fd;
	char	**args;
	char	*path;
	int		arg_count;
	int		status;
	pid_t	pid;
	int		flags;
	int		heredoc_pipe[2];

	args = NULL;
	last_input_fd = input_fd;
	last_output_fd = output_fd;
	arg_count = 0;
	current = exec_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == REDIRECTION)
		{
			if (ft_strcmp(token->str, "<<") == 0)
			{
				if (pipe(heredoc_pipe) == -1)
				{
					ft_fprintf(2, "Error creating pipe for heredoc\n");
					return;
				}
				write(heredoc_pipe[1], token->detail.rdrc.rdrcmeta, ft_strlen(token->detail.rdrc.rdrcmeta));
				close(heredoc_pipe[1]);
				if (last_input_fd != input_fd)
					close(last_input_fd);
				last_input_fd = heredoc_pipe[0];
			}
			else if (ft_strcmp(token->str, "<") == 0)
			{
				if (last_input_fd != input_fd)
					close(last_input_fd);
				last_input_fd = open(token->detail.rdrc.rdrcmeta, O_RDONLY);
				if (last_input_fd == -1)
				{
					ft_fprintf(2, "Error opening input file: %s\n", token->detail.rdrc.rdrcmeta);
					return ;
				}
			}
			else if (ft_strcmp(token->str, ">") == 0 || ft_strcmp(token->str, ">>") == 0)
			{
				if (last_output_fd != output_fd)
					close(last_output_fd);
				if (ft_strcmp(token->str, ">") == 0)
					flags = O_WRONLY | O_CREAT | O_TRUNC;
				else
					flags = O_WRONLY | O_CREAT | O_APPEND;
				last_output_fd = open(token->detail.rdrc.rdrctarget, flags, 0644);
				if (last_output_fd == -1)
				{
					ft_fprintf(2, "Error opening output file: %s\n", token->detail.rdrc.rdrctarget);
					return ;
				}
			}
		}
		else if (token->token == COMMAND || token->token == WORD)
		{
			arg_count++;
			char **temp = ft_calloc(arg_count + 1, sizeof(char *));
			if (!temp)
			{
				ft_fprintf(2, "Error: Memory allocation failed\n");
				ft_array_l_free(args, arg_count - 1);
				return ;
			}
			if (args)
			{
				for (int i = 0; i < arg_count - 1; i++)
					temp[i] = args[i];
				free(args);
			}
			args = temp;
			args[arg_count - 1] = token->str;
			args[arg_count] = NULL;
		}
		current = current->next;
	}
	if (args && args[0])
	{
		if (is_builtin(args[0]))
		{
			if (last_input_fd != input_fd)
				dup2(last_input_fd, STDIN_FILENO);
			if (last_output_fd != output_fd)
				dup2(last_output_fd, STDOUT_FILENO);
			execute_builtin(m, args[0], args, arg_count);
			if (ft_strcmp(args[0], "exit") == 0)
				exit(m->exitcode);
		}
		else
		{
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
	}
	if (last_input_fd != input_fd)
		close(last_input_fd);
	if (last_output_fd != output_fd)
		close(last_output_fd);
	ft_array_l_free(args, arg_count);
}

void	execute(t_minishell *m)
{
	int		status;
	int		i;
	int		pipe_fd[2];
	int		prev_pipe_read;
	pid_t	*pids;
	int		j;

	if (DEBUG == 1)
	{
		ft_printf(Y"execute:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	prev_pipe_read = STDIN_FILENO;
	i = 0;
	j = 0;
	m->pipes = 0;
	split_pipes(m, &(m->cmd_seqs), &(m->exec_seqs));
	if (m->pipes != 0)
	{
		if (DEBUG == 1)
			ft_printf(G"PIPE SEGMENTS:\n"D);
		pids = malloc(sizeof(pid_t) * (m->pipes + 1));
		if (!pids)
		{
			ft_fprintf(2, "Memory allocation failed\n");
			return ;
		}
		while (m->cmd_seqs[i] != NULL)
		{
			if (DEBUG == 1)
			{
				ft_printf(Y"PIPE:\n"D);
				ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
			}
			prexecute(m, &(m->cmd_seqs[i]), &(m->exec_seqs[i]));
			if (DEBUG == 1)
			{
				ft_printf(Y"cmd_seqs:\n"D);
				ft_lstput(&(m->cmd_seqs[i]), put_token, '\n');
				ft_printf(Y"exec_seqs:\n"D);
				ft_lstput(&(m->exec_seqs[i]), put_token, '\n');
			}
			if (m->cmd_seqs[i + 1] != NULL)
			{
				if (pipe(pipe_fd) == -1)
				{
					ft_fprintf(2, "Pipe creation failed\n");
					free(pids);
					return ;
				}
			}
			else
			{
				pipe_fd[1] = STDOUT_FILENO;
			}
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			pids[i] = fork();
			if (pids[i] == 0)
			{
				signal(SIGINT, handle_child_process);
				signal(SIGQUIT, handle_child_process);
				if (i > 0)
				{
					dup2(prev_pipe_read, STDIN_FILENO);
					close(prev_pipe_read);
				}
				if (m->cmd_seqs[i + 1] != NULL)
				{
					close(pipe_fd[0]);
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
				}
				run_seg(m, m->exec_seqs[i], STDIN_FILENO, STDOUT_FILENO);
				exit(0);
			}
			else if (pids[i] < 0)
			{
				ft_fprintf(2, "Fork failed\n");
				free(pids);
				return ;
			}
			if (i > 0)
				close(prev_pipe_read);
			if (m->cmd_seqs[i + 1] != NULL)
			{
				close(pipe_fd[1]);
				prev_pipe_read = pipe_fd[0];
			}
			i++;
		}
		while (j < i)
		{
			waitpid(pids[j], &status, 0);
			signal(SIGINT, handle_main_process);
			signal(SIGQUIT, handle_main_process);
			if (WIFEXITED(status))
				m->exitcode = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				m->exitcode = 128 + WTERMSIG(status);
			j++;
		}
		free(pids);
	}
	else
	{
		if (DEBUG == 1)
			ft_printf(G"EXEC without pipe\n"D);
		prexecute(m, &(m->tok_lst), &(m->exec_lst));
		run_seg(m, m->tok_lst, STDIN_FILENO, STDOUT_FILENO);
	}
	reset_sequences(m);
}
