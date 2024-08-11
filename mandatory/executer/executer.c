/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/11 21:55:39 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "./../../include/minishell.h"

char	*get_executable(t_minishell *m, char *command)
{
	char	*pathline;
	char	*joined;
	char	**paths;
	int		pathcount;
	int		i;

	pathline = my_getenv("PATH", m->env_list);
	if (pathline != NULL)
	{
		pathcount = ft_count_words(pathline, ':');
		paths = ft_split(pathline, ':');
		i = 0;
		while (i < pathcount)
		{
			joined = ft_strjoin(ft_strjoin(paths[i++], "/"), command);
			if (joined == NULL)
				break ;
			if (access(joined, X_OK) == 0)
				return (ft_array_l_free(paths, pathcount), joined);
			free(joined);
		}
		ft_array_l_free(paths, pathcount);
	}
	return (NULL);
}

//signal(SIGINT, handle_child_process);
//signal(SIGQUIT, handle_child_process);
//signal(SIGINT, handle_main_process);
//signal(SIGQUIT, handle_main_process);
void	execute_command(t_minishell *m, char *executable, char **argv)
{
	execve(executable, argv, own_env(m->env_list));
	perror("execve failed");
	free(executable);
	exit(EXIT_FAILURE);
}

static int	append_str(char **dst, const char *src)
{
	char	*temp;

	temp = ft_realloc(*dst, ft_strlen(*dst) + ft_strlen(src) + 1);
	if (!temp)
		return (0);
	*dst = temp;
	ft_strcat(*dst, src);
	return (1);
}

static int	append_char(char **dst, char c)
{
	char	*temp;
	size_t	len;

	temp = ft_realloc(*dst, ft_strlen(*dst) + 2);
	if (!temp)
		return (0);
	*dst = temp;
	len = ft_strlen(*dst);
	(*dst)[len] = c;
	(*dst)[len + 1] = '\0';
	return (1);
}

// here the content of a heredoc is expanded in a different way than the tokenizer expands
// this way is sufficient for heredocs and less complicated
char	*expand_hd(t_minishell *m, char *str)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	size_t	i;
	char	*exit_status_str;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			exit_status_str = ft_itoa(m->last_exitcode);
			if (!exit_status_str || !append_str(&result, exit_status_str))
			{
				free(exit_status_str);
				free(result);
				return (NULL);
			}
			free(exit_status_str);
			i++;
		}
		else if (str[i] == '$')
		{
			var_name = get_var_name(str, &i);
			if (var_name)
			{
				var_value = my_getenv(var_name, m->env_list);
				if (var_value && !append_str(&result, var_value))
				{
					free(var_name);
					free(result);
					return (NULL);
				}
				free(var_name);
			}
			else
			{
				if (!append_char(&result, '$'))
				{
					free(result);
					return (NULL);
				}
			}
		}
		else if (str[i] != '"')
		{
			if (!append_char(&result, str[i]))
			{
				free(result);
				return (NULL);
			}
		}
		i++;
	}
	return (result);
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

	line_len = ft_strlen(line);
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
	token->detail.rdrc.rdrcmeta = filecontent;
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
		pid = fork();
		if (pid == 0)
		{
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
			if (is_builtin(args[0]))
			{
				execute_builtin(m, args[0], args, arg_count);
				exit(0);
			}
			else
			{
				path = get_executable(m, args[0]);
				if (path)
				{
					execute_command(m, path, args);
					free(path);
				}
				ft_fprintf(2, "Command not found: %s\n", args[0]);
				exit(1);
			}
		}
		else if (pid < 0)
		{
			ft_fprintf(2, "Fork failed\n");
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				m->exitcode = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				m->exitcode = 128 + WTERMSIG(status);
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
			pids[i] = fork();
			if (pids[i] == 0)
			{
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
}

