/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/05 23:09:22 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

t_list	**split_by_pipe(t_minishell *m)
{
	t_list	*current;
	t_list	*sequence;
	t_list	**cmd_sequences;
	int		sequence_count;
	t_token	*token;

	current = m->tok_lst;
	sequence = NULL;
	cmd_sequences = NULL;
	sequence_count = 0;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
		{
			if (sequence != NULL)
			{
				cmd_sequences = ft_realloc(cmd_sequences, sizeof(t_list *) * (sequence_count + 1));
				if (cmd_sequences == NULL)
				{
					perror("realloc");
					exit(EXIT_FAILURE);
				}
				cmd_sequences[sequence_count++] = sequence;
				sequence = NULL;
			}
			m->pipes++;
		}
		else
		{
			add_token_to_list(&sequence, token);
		}
		current = current->next;
	}
	if (sequence != NULL)
	{
		cmd_sequences = ft_realloc(cmd_sequences, sizeof(t_list *) * (sequence_count + 1));
		if (cmd_sequences == NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		cmd_sequences[sequence_count++] = sequence;
		sequence = NULL;
	}
	cmd_sequences = ft_realloc(cmd_sequences, sizeof(t_list *) * (sequence_count + 1));
	if (cmd_sequences == NULL)
	{
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	cmd_sequences[sequence_count] = NULL;
	return (cmd_sequences);
}

static char	*get_executable(t_minishell *m, char *command)
{
	char	*pathline;
	char	*joined;
	char	*temp;
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
			temp = ft_strjoin(paths[i], "/");
			joined = ft_strjoin(temp, command);
			free(temp);
			if (joined == NULL)
				break ;
			if (access(joined, X_OK) == 0)
				return (ft_array_l_free(paths, pathcount), joined);
			i++;
			free(joined);
		}
		ft_array_l_free(paths, pathcount);
	}
	return (NULL);
}

static char	**own_env(t_envlst *env_lst)
{
	size_t		k;
	char		**env;

	k = 0;
	env = (char **)malloc(sizeof(char *) * (count_list(env_lst) + 1));
	if (!env)
		return (NULL);
	while (env_lst != NULL)
	{
		if (env_lst->value != NULL)
		{
			env[k] = ft_strjoin(ft_strjoin(env_lst->name, "="), env_lst->value);
			if (!env[k])
			{
				while (k > 0)
					free(env[--k]);
				return (free(env), NULL);
			}
			k++;
		}
		env_lst = env_lst->next;
	}
	env[k] = NULL;
	return (env);
}

void	handle_error(t_minishell *m, int code, char *message)
{
	ft_fprintf(2, R"%s\n"D, message);
	m->exitcode = code;
}

void	ft_run_others(t_minishell *m, char *command, char **argv)
{
	char	*executable;
	char	*message;
	pid_t	pid;
	int		status;

	message = ft_strjoin(ft_strjoin("bash: ", command), ": command not found");
	if (access(command, X_OK) == 0)
		executable = command;
	else
		executable = get_executable(m, command);
	if (!executable)
	{
		handle_error(m, 127, message);
		free(message);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		handle_error(m, 1, "Fork failed");
		free(executable);
		free(message);
		return;
	}
	else if (pid == 0)
	{
		execve(executable, argv, own_env(m->env_list));
		perror("execve failed");
		free(executable);
		free(message);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			m->exitcode = WEXITSTATUS(status);
		}
	}
	free(executable);
	free(message);
}

void run_command(t_minishell *m, t_list *current)
{
	t_token		*token;
	t_envlst	**envlist;
	t_list		*arg_list;
	int			i;
	int			argc;
	int			noword;
	char		**argv;
	char		*command;

	i = 0;
	argc = 0;
	noword = 0;
	if (current == NULL)
		return ;
	envlist = &(m->env_list);
	token = (t_token *)current->content;
	command = whitespace_handler(token->str);
	current = current->next;
	arg_list = current;
	noword = 0;
	while (arg_list != NULL && noword == 0)
	{
		token = (t_token *)arg_list->content;
		if (token->token == WORD)
			argc++;
		else
			noword = 1;
		arg_list = arg_list->next;
	}
	argv = (char **)malloc((argc + 2) * sizeof(char *));
	if (!argv)
		return (perror("malloc failed"));
	argv[i++] = ft_strdup(command);
	if (!argv[0])
		return (perror("ft_strdup failed"), free(argv));
	noword = 0;
	while (current != NULL && noword == 0)
	{
		token = (t_token *)current->content;
		if (token->token == WORD)
		{
			argv[i] = ft_strdup(token->str);
			if (!argv[i])
			{
				perror("ft_strdup failed");
				while (i > 0)
					free(argv[--i]);
				return (free(argv));
			}
			i++;
		}
		else
			noword = 1;
		current = current->next;
	}
	argv[i] = NULL;
	if (ft_strcmp(command, "cd") == 0)
		ft_cd(i, argv, &envlist);
	else if (!ft_strcmp(command, "echo"))
		ft_echo(argv);
	else if (!ft_strcmp(command, "env"))
		ft_env(*envlist);
	else if (!ft_strcmp(command, "exit"))
		ft_exit(argv);
	else if (!ft_strcmp(command, "export"))
		ft_export(i, argv, &envlist);
	else if (!is_var_name(*envlist, &argv[0]))
		ft_export(i, argv, &envlist);
	else if (!ft_strcmp(command, "pwd"))
		ft_pwd(argv);
	else if (!ft_strcmp(command, "unset"))
		ft_unset(envlist, argv);
	else
		ft_run_others(m, command, argv);
	i = 0;
	while (argv[i] != NULL)
		free(argv[i++]);
	if (DEBUG == 1)
		ft_printf(B"exitcode: %i\n"D, m->exitcode);
	free(argv);
}

void	check_semantics(t_list *last, t_list *current)
{
	t_token	*last_token;
	t_token	*token;

	last_token = NULL;
	token = (t_token *)current->content;
	if (last)
	{
		last_token = (t_token *)last->content;
		if (!ft_strcmp(token->type, "Word")
			&& !ft_strcmp(last_token->type, "Redirection")
			&& !ft_strcmp(last_token->str, "<<"))
			update_tok_type(token, DELIMITER);
		if (!ft_strcmp(token->type, "Word")
			&& !ft_strcmp(last_token->type, "Pipe")
			&& !ft_strcmp(last_token->str, "|"))
			update_tok_type(token, COMMAND);
	}
	else
		if (!ft_strcmp(token->type, "Word"))
			update_tok_type(token, COMMAND);
}

void	pre_exec_prep(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0)
			cur_content = init_pipe_details(cur_content);
		else if (ft_strcmp(cur_content->type, "Redirection") == 0)
			init_redirection_details(cur_content);
		current = current->next;
	}
}

void	check_pipes(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;
	char	*last_type;

	last_type = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0 && last_type == NULL)
			handle_error(m, 2, "bash: syntax error near unexpected token `|'");
		else if (current->next == NULL && !ft_strcmp(cur_content->type, "Pipe"))
			cur_content->detail.pipe.open_prompt = 1;
		last_type = cur_content->type;
		current = current->next;
	}
}

void	set_rdrctype(t_list *last, t_list *current, t_token *token)
{
	check_semantics(last, current);
	if (ft_strcmp(token->type, "Redirection") == 0)
	{
		if (ft_strcmp(token->str, "<<") == 0)
			token->detail.redirection.rdrctype = ft_strdup("here_doc");
		if (ft_strcmp(token->str, ">>") == 0)
			token->detail.redirection.rdrctype = ft_strdup("append");
		if (ft_strcmp(token->str, "<") == 0)
			token->detail.redirection.rdrctype = ft_strdup("redirection");
		if (ft_strcmp(token->str, ">") == 0)
			token->detail.redirection.rdrctype = ft_strdup("truncate");
	}
}

void	check_redirections(t_minishell *m)
{
	t_list	*current;
	t_list	*last;
	t_token	*cur_content;
	char	*last_type;
	char	*last_str;

	last = NULL;
	last_type = NULL;
	last_str = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0 && ft_strcmp(last_type, "Redirection") == 0)
			handle_error(m, 2, "bash: syntax error near unexpected token `|'");
		else if (current->next == NULL && ft_strcmp(cur_content->type, "Redirection") == 0)
			handle_error(m, 2, "bash: syntax error near unexpected token `newline'");
		else if (ft_strcmp(last_type, "Redirection") == 0 && ft_strcmp(cur_content->type, "Redirection") == 0)
		{
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, ">") == 0)
				handle_error(m, 2, "bash: syntax error near unexpected token `>'");
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, ">>") == 0)
				handle_error(m, 2, "bash: syntax error near unexpected token `>>'");
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, "<") == 0)
				handle_error(m, 2, "bash: syntax error near unexpected token `<'");
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, "<<") == 0)
				handle_error(m, 2, "bash: syntax error near unexpected token `<<'");
		}
		set_rdrctype(last, current, cur_content);
		last_type = cur_content->type;
		last_str = cur_content->str;
		last = current;
		current = current->next;
	}
}

void	pre_exec_checks(t_minishell *m)
{
	check_pipes(m);
	check_redirections(m);
}

char	*get_last_cmd(t_list *ref, t_list *item)
{
	t_list	*current;
	t_token	*cur_content;
	char	*last_cmd;

	last_cmd = NULL;
	current = ref;
	while (current != NULL && current != item)
	{
		cur_content = (t_token *)current->content;
		if (cur_content != NULL)
		{
			if (cur_content->token == COMMAND)
			{
				last_cmd = cur_content->str;
			}
		}
		current = current->next;
	}
	return last_cmd;
}

int handle_redirections(t_minishell *m, t_list *ref, t_list *seq)
{
	int		fd = -1;
	t_token	*token;
	char	*file_name = NULL;
	int		pipe_fd[2];

	while (seq != NULL) {
		token = (t_token *)seq->content;
		if (token->token == REDIRECTION)
		{
			if (ft_strcmp(token->detail.redirection.rdrctype, "truncate") == 0 || ft_strcmp(token->detail.redirection.rdrctype, "append") == 0)
				{
				if (seq->next == NULL)
					break ;
				seq = seq->next;
				if (seq != NULL)
				{
					token = (t_token *)seq->content;
					if (token->token == WORD)
					{
						if (file_name != NULL)
						{
							free(file_name);
						}
						file_name = ft_strdup(token->str);
					}
				}
				if (seq->next == NULL)
					break ;
				seq = seq->next;
				while (seq != NULL)
				{
					token = (t_token *)seq->content;
					handle_error(m, 2, ft_strjoin(ft_strjoin(ft_strjoin(get_last_cmd(ref, seq), ": "), token->str), ": No such file or directory"));
					if (token->token == WORD)
					{
						if (file_name != NULL)
						{
							free(file_name);
						}
						file_name = strdup(token->str);
					}
					else
					{
						break ;
					}
					seq = seq->next;
				}
				if (file_name == NULL)
				{
					fprintf(stderr, "No file name specified for redirection\n");
					return -1;
				}
				if (ft_strcmp(token->detail.redirection.rdrctype, "truncate"))
					fd = open(file_name, (O_WRONLY | O_CREAT | O_TRUNC), 0644);
				else
					fd = open(file_name, (O_WRONLY | O_CREAT | O_APPEND), 0644);
				if (fd == -1)
				{
					perror("Failed to open file for writing (truncate/append)");
					free(file_name);
					return -1;
				}
				if (dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("dup2 failed for stdout redirection");
					close(fd);
					free(file_name);
					return -1;
				}
				close(fd);

				free(file_name);
				file_name = NULL;
			}
			else if (ft_strcmp(token->detail.redirection.rdrctype, "redirection") == 0)
			{
				seq = seq->next;
				if (seq != NULL) {
					token = (t_token *)seq->content;
					if (token->token == WORD)
						file_name = token->str;
					else
					{
						fprintf(stderr, "Expected a file name after redirection symbol\n");
						return -1;
					}
					fd = open(file_name, O_RDONLY);
					if (fd == -1)
					{
						perror("Failed to open file for reading");
						return -1;
					}
					if (dup2(fd, STDIN_FILENO) == -1)
					{
						perror("dup2 failed for stdin redirection");
						close(fd);
						return -1;
					}
					close(fd);
				}
			} else if (ft_strcmp(token->detail.redirection.rdrctype, "here_doc") == 0)
			{
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe failed for heredoc");
					return (-1);
				}
				write(pipe_fd[1], token->str, strlen(token->str));
				close(pipe_fd[1]);
				if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
				{
					perror("dup2 failed for heredoc stdin");
					close(pipe_fd[0]);
					return (-1);
				}
				close(pipe_fd[0]);
			}
		}
		else
		{
			seq = seq->next;
		}
	}
	return (0);
}


void	execute(t_minishell *m)
{
	int		pipe_fd[2];
	int		prev_pipe_read;
	pid_t	pid;
	int		status;
	int		i;
	t_list	*seq;

	prev_pipe_read = -1;
	i = 0;
	m->pipes = 0;
	pre_exec_prep(m);
	pre_exec_checks(m);
	if (DEBUG == 1)
	{
		ft_printf(Y"EXECUTE:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	m->cmd_sequences = split_by_pipe(m);
	if (m->pipes != 0)
	{
		if (DEBUG == 1)
			ft_printf(Y"PIPE SEGMENTS:\n"D);
		while (m->cmd_sequences[i] != NULL)
		{
			if (DEBUG == 1)
			{
				ft_printf(Y"seg %i:\n"D, i);
				ft_lstput(&(m->cmd_sequences[i]), put_token, '\n');
			}
			seq = m->cmd_sequences[i];
			if (m->cmd_sequences[i + 1] != NULL)
			{
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe failed");
					exit(EXIT_FAILURE);
				}
			}
			pid = fork();
			if (pid == -1)
			{
				perror("fork failed");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0)
			{
				if (prev_pipe_read != -1)
				{
					if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
					{
						perror("dup2 failed for stdin");
						exit(EXIT_FAILURE);
					}
					close(prev_pipe_read);
				}
				if (m->cmd_sequences[i + 1] != NULL)
				{
					close(pipe_fd[0]);
					if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
					{
						perror("dup2 failed for stdout");
						exit(EXIT_FAILURE);
					}
					close(pipe_fd[1]);
				}
				if (handle_redirections(m, m->cmd_sequences[i], seq) == -1)
					exit(EXIT_FAILURE);
				run_command(m, seq);
				exit(m->exitcode);
			}
			else
			{
				if (prev_pipe_read != -1)
					close(prev_pipe_read);
				if (m->cmd_sequences[i + 1] != NULL)
				{
					close(pipe_fd[1]);
					prev_pipe_read = pipe_fd[0];
				}
			}
			i++;
		}
	}
	else
	{
		if (handle_redirections(m, m->tok_lst, m->tok_lst) == -1)
			return ;
		run_command(m, m->tok_lst);
	}
	while ( wait(&status) > 0 )
	{
		ft_printf("waiting ... \n");
	}
	if (WIFEXITED(status))
	{
		m->exitcode = WEXITSTATUS(status);
	}
}
