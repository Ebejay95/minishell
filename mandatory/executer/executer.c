/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/12 16:38:12 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	reset_sequences(t_minishell *m)
{
	int	i;

	ft_printf("reset_sequences %i\n", m->exitcode);
	if (m->exec_seqs)
	{
		i = 0;
		while (m->exec_seqs[i])
		{
			ft_lstclear(&(m->exec_seqs[i]), free_token);
			i++;
		}
		m->exec_seqs = NULL;
	}
	//ft_lstclear(&(m->tok_lst), free_token);
	ft_lstclear(&(m->exec_lst), free_token);
}

char	*find_executable_in_paths(char **paths, int pathcount, char *command)
{
	int		i;
	char	*joined;
	char	*full_path;

	i = 0;
	while (i < pathcount)
	{
		full_path = ft_strjoin(paths[i], "/");
		if (full_path == NULL)
			return (NULL);
		joined = ft_strjoin(full_path, command);
		free(full_path);
		if (joined == NULL)
			return (NULL);
		if (access(joined, X_OK) == 0)
			return (joined);
		free(joined);
		i++;
	}
	return (NULL);
}

char	*get_executable(t_minishell *m, char *command)
{
	char	*pathline;
	char	**paths;
	int		pathcount;
	char	*result;

	if (ft_strchr(command, '/') && access(command, F_OK) == 0)
		return (ft_strdup(command));
	pathline = my_getenv("PATH", m->env_list);
	if (pathline != NULL)
	{
		pathcount = ft_count_words(pathline, ':');
		paths = ft_split(pathline, ':');
		if (paths == NULL)
			return (NULL);
		result = find_executable_in_paths(paths, pathcount, command);
		ft_array_l_free(paths, pathcount);
		return (result);
	}
	return (NULL);
}

void	execute_command(t_minishell *m, char *executable, char **argv)
{
	if (is_builtin(executable))
		execute_builtin(m, executable, argv, ft_array_length(argv));
	else
	{
		execve(executable, argv, own_env(m->env_list));
		perror("execve failed");
		free(executable);
		exit(EXIT_FAILURE);
	}
}

void	handle_heredoc(t_minishell *m, t_list *current)
{
	t_token	*next_content;
	t_token	*token;
	char	*heredoc_content;
	size_t	content_size;
	size_t	content_capacity;
	char	*line;
	char	*delimiter;
	size_t	line_len;
	char	*new_content;

	token = (t_token *)current->content;
	heredoc_content = NULL;
	content_size = 0;
	content_capacity = 0;
	next_content = NULL;
	if (current->next)
		next_content = (t_token *)current->next->content;
	delimiter = next_content->str;
	while (1)
	{
		line = readline("📄 ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line_len = ft_strlen(line);
		if (content_size + line_len + 2 > content_capacity)
		{
			if (content_capacity)
				content_capacity = content_capacity * 2;
			else
				content_capacity = 1024;
			new_content = realloc(heredoc_content, content_capacity);
			if (!new_content)
			{
				free(heredoc_content);
				free(line);
			}
			heredoc_content = new_content;
		}
		ft_strcpy(heredoc_content + content_size, line);
		content_size += line_len;
		heredoc_content[content_size++] = '\n';
		heredoc_content[content_size] = '\0';
		free(line);
	}
	if (!next_content->had_quote)
		token->detail.rdrc.rdrcmeta = expand_hd(m, heredoc_content);
	else
		token->detail.rdrc.rdrcmeta = heredoc_content;
}

void	handle_infile(t_list *current)
{
	t_token	*next_content;
	char	*filename;
	char	*line;
	char	*temp;
	size_t	total_size;
	char	*filecontent;
	t_token	*token;
	int		fd;
	size_t	line_len;

	// line_len = ft_strlen(line); // DAS HIER HAT FUER DEN SEGFAULT GESORGT
	token = (t_token *)current->content;
	total_size = 0;
	filecontent = NULL;
	next_content = NULL;
	if (current->next)
		next_content = (t_token *)current->next->content;
	filename = next_content->str;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "Error: Cannot open file '%s'\n", filename);
		return ;
	}
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
	token->detail.rdrc.rdrcmeta = filename;
}

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

int	keep_for_exec(t_token *token)
{
	if (token->token == WORD || token->token == COMMAND || token->token == PIPE)
		return (1);
	return (0);
}

void	prexecute(t_minishell *m, t_list **tok_lst, t_list **exec_lst)
{
	t_list	*current;
	t_token	*token;

	if (DEBUG == 1)
	{
		ft_printf(Y"SEMANTIC TOKENIZE:\n"D);
		ft_lstput(tok_lst, put_token, '\n');
	}
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
	if (DEBUG == 1)
	{
		ft_printf(R"FOR EXECUTE:\n"D);
		ft_lstput(exec_lst, put_token, '\n');
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
			if (ft_strcmp(token->str, "<") == 0 || ft_strcmp(token->str, "<<") == 0)
			{
				if (last_input_fd != input_fd)
					close(last_input_fd);
				// printf("token->detail.rdrc.rdrcmeta: %s\n", token->detail.rdrc.rdrcmeta);
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
			args = ft_realloc(args, sizeof(char *) * (arg_count + 1));
			args[arg_count - 1] = token->str;
			args[arg_count] = NULL;
		}
		current = current->next;
	}
	if (args && args[0])
	{
		if (is_builtin(args[0]))
		{
			// Handle builtins in the parent process
			if (last_input_fd != input_fd)
				dup2(last_input_fd, STDIN_FILENO);
			if (last_output_fd != output_fd)
				dup2(last_output_fd, STDOUT_FILENO);
			execute_builtin(m, args[0], args, arg_count);
			if (ft_strcmp(args[0], "exit") == 0)
				exit(m->exitcode); // Exit the shell if the command was 'exit'
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
				ft_fprintf(2, "Command not found: %s\n", args[0]);
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
	free(args);
}

void	execute(t_minishell *m)
{
	int		status;
	int		i;
	int		pipe_fd[2];
	int		prev_pipe_read;
	pid_t	*pids;
	int		j;

	prev_pipe_read = STDIN_FILENO;
	i = 0;
	j = 0;
	m->pipes = 0;
	split_by_pipe(m, &(m->cmd_seqs), &(m->exec_seqs));
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
			prexecute(m, &(m->cmd_seqs[i]), &(m->exec_seqs[i]));
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
			printf("pids[%i] = %i\n", i, pids[i]);
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
		run_seg(m, m->exec_lst, STDIN_FILENO, STDOUT_FILENO);
	}
	reset_sequences(m);
}
