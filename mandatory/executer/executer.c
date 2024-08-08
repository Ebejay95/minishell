/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/09 00:52:32 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"


// # USED TEXT MESSAGES:
// 3: bash: syntax error near unexpected token `>'
// 4: bash: syntax error near unexpected token `>>'
// 5: bash: syntax error near unexpected token `<'
// 6: bash: syntax error near unexpected token `<<'
static void	check_rdrc_norm(char *last_str, t_token *cont, t_minishell *m)
{
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, ">"))
		pic_err(m, 2, text(3));
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, ">>"))
		pic_err(m, 2, text(4));
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, "<"))
		pic_err(m, 2, text(5));
	if ((!ft_strcmp(last_str, "<<") || !ft_strcmp(last_str, ">>"))
		&& !ft_strcmp(cont->str, "<<"))
		pic_err(m, 2, text(6));
}

// # USED TEXT MESSAGES:
// #2: bash: syntax error near unexpected token `|'
// #8: Pipe
static void	check_pipes(t_minishell *m)
{
	t_list	*current;
	t_token	*cont;
	char	*end;
	char	*last_str;
	char	*ltype;

	end = NULL;
	last_str = NULL;
	ltype = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cont = (t_token *)current->content;
		if (!m->leave && !ft_strcmp(ltype, text(8)) && !ft_strcmp(cont->type, text(8)))
			pic_err(m, 2, text(2));
		else if (!m->leave && ft_strcmp(cont->type, text(8)) == 0 && end == NULL)
			pic_err(m, 2, text(2));
		else if (!m->leave && current->next == NULL && !ft_strcmp(cont->type, text(8)))
			pic_err(m, 2, text(1));
		end = cont->type;
		last_str = cont->str;
		ltype = cont->type;
		current = current->next;
	}
}

void	red_need_next_file(t_minishell *m, t_list *cur, t_token *cont)
{
	t_token	*cont_next;
	int		lookat;

	lookat = 0;
	cont_next = NULL;
	if (!ft_strcmp(cont->str, "<") || !ft_strcmp(cont->str, ">>") || !ft_strcmp(cont->str, ">"))
		lookat = 1;
	if (lookat && cur->next)
	{
		cont_next = (t_token *)cur->next->content;
		if (access(cont_next->str, R_OK) != 0)
			pic_err(m, 2, ft_strjoin(ft_strjoin("bash: ", cont_next->str), ": No such file or directory"));
	}
}

// # USED TEXT MESSAGES:
// #1: bash: syntax error near unexpected token `newline'
// #2: bash: syntax error near unexpected token `|'
// #7: Redirection
// #8: Pipe
static void	check_redirections(t_minishell *m)
{
	t_list	*cur;
	t_list	*last;
	t_token	*cont;
	char	*end;
	char	*last_str;

	last = NULL;
	end = NULL;
	last_str = NULL;
	cur = m->tok_lst;
	while (cur != NULL)
	{
		cont = (t_token *)cur->content;
		if (!m->leave && !ft_strcmp(cont->type, text(8)) && !ft_strcmp(end, text(7)))
			pic_err(m, 2, text(2));
		else if (!m->leave && cur->next == NULL && !ft_strcmp(cont->type, text(7)))
			pic_err(m, 2, text(1));
		else if (!m->leave && !ft_strcmp(end, text(7)) && !ft_strcmp(cont->type, text(7)))
			check_rdrc_norm(last_str, cont, m);
		set_rdrctype(last, cur, cont);
		end = cont->type;
		last_str = cont->str;
		last = cur;
		cur = cur->next;
	}
}

void	pre_exec_checks(t_minishell *m)
{
	if (!m->leave)
		check_pipes(m);
	if (!m->leave)
		check_redirections(m);
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

int	is_builtin(char *command)
{
	return (!ft_strcmp(command, "cd")
		|| !ft_strcmp(command, "echo")
		|| !ft_strcmp(command, "env")
		|| !ft_strcmp(command, "exit")
		|| !ft_strcmp(command, "export")
		|| !ft_strcmp(command, "pwd")
		|| !ft_strcmp(command, "unset"));
}

void	execute_builtin(t_minishell *m, char *com, char **argv, int argc)
{
	if (!ft_strcmp(com, "cd"))
		ft_cd(argc, argv, &m->env_list);
	else if (!ft_strcmp(com, "echo"))
		ft_echo(argv);
	else if (!ft_strcmp(com, "env"))
		ft_env(m->env_list);
	else if (!ft_strcmp(com, "exit"))
		ft_exit(argv, &m->exitcode);
	else if (!ft_strcmp(com, "export"))
		ft_export(argc, argv, &m->env_list);
	else if (!ft_strcmp(com, "pwd"))
		ft_pwd(argv);
	else if (!ft_strcmp(com, "unset"))
		ft_unset(&m->env_list, argv);
}

void	pic_var_or_others(t_minishell *m, char *com, char **argv, int argc)
{
	if (!is_var_name(m->env_list, &argv[0]))
		ft_export(argc, argv, &m->env_list);
	else
		ft_run_others(m, com, argv);
}

void	cleanup(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		free(argv[i++]);
	free(argv);
}

int	is_word_token(t_list *node)
{
	t_token	*token;

	token = (t_token *)node->content;
	return (token->token == WORD);
}

void run_command(t_minishell *m, t_list *current)
{
	t_token	*token;
	char	*command;
	char	**argv;
	int		argc;

	if (current == NULL)
		return ;
	token = (t_token *)current->content;
	command = whitespace_handler(token->str);
	argv = prepare_argv(current, &argc);
	if (!argv)
		return ;

	m->exitcode = 0;
	if (is_builtin(command))
		execute_builtin(m, command, argv, argc);
	else
		pic_var_or_others(m, command, argv, argc);
	m->last_exitcode = m->exitcode;
	cleanup(argv);
}

char	**prepare_argv(t_list *current, int *argc)
{
	char	**argv;
	int		capacity;
	t_list	*temp;

	*argc = 0;
	capacity = 10;
	argv = (char **)malloc((capacity + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	if (!add_first_arg(argv, current, argc))
		return (NULL);
	temp = current->next;
	while (temp && is_word_token(temp))
	{
		if (!add_arg(&argv, &capacity, temp, argc))
			return (NULL);
		temp = temp->next;
	}
	argv[*argc] = NULL;
	return (argv);
}

int	add_first_arg(char **argv, t_list *current, int *argc)
{
	t_token	*token;

	token = (t_token *)current->content;
	argv[0] = ft_strdup(token->str);
	if (!argv[0])
		return (free(argv), 0);
	*argc = 1;
	return (1);
}

int	add_arg(char ***argv, int *capacity, t_list *temp, int *argc)
{
	t_token	*token;

	if (*argc >= *capacity)
		if (!resize_argv(argv, capacity))
			return (0);
	token = (t_token *)temp->content;
	(*argv)[*argc] = ft_strdup(token->str);
	if (!(*argv)[*argc])
		return (cleanup(*argv), 0);
	(*argc)++;
	return (1);
}

int	resize_argv(char ***argv, int *capacity)
{
	char	**new_argv;

	*capacity *= 2;
	new_argv = (char **)realloc(*argv, (*capacity + 1) * sizeof(char *));
	if (!new_argv)
		return (cleanup(*argv), 0);
	*argv = new_argv;
	return (1);
}

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

char	**own_env(t_envlst *env_lst)
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

void	ft_error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	pic_err(t_minishell *m, int code, char *mes)
{
	ft_fprintf(2, R"%s\n"D, mes);
	m->leave = 1;
	m->exitcode = code;
}

char	*text(int message)
{
	if (message == 1)
		return ("bash: syntax error near unexpected token `newline'");
	if (message == 2)
		return ("bash: syntax error near unexpected token `|'");
	if (message == 3)
		return ("bash: syntax error near unexpected token `>'");
	if (message == 4)
		return ("bash: syntax error near unexpected token `>>'");
	if (message == 5)
		return ("bash: syntax error near unexpected token `<'");
	if (message == 6)
		return ("bash: syntax error near unexpected token `<<'");
	if (message == 7)
		return ("Redirection");
	if (message == 8)
		return ("Pipe");
	if (message == 9)
		return ("Word");
	if (message == 10)
		return ("No file name specified for redirection\n");
	if (message == 11)
		return ("Failed to open file for writing (truncate/append)");
	if (message == 12)
		return ("numeric argument required");
	return (NULL);
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
	return (last_cmd);
}

void	ft_run_others(t_minishell *m, char *command, char **argv)
{
	char	*executable;
	char	*mes;

	executable = prepare_executable_and_message(m, command);
	if (!executable)
	{
		mes = ft_strjoin(ft_strjoin("bash: ", command), ": command not found");
		pic_err(m, 127, mes);
		free(mes);
		return ;
	}
	execute_command(m, executable, argv);
}

t_token	*init_pipe_details(t_token *pipetok)
{
	pipetok->detail.pipe.fdin = -1;
	pipetok->detail.pipe.fdout = -1;
	pipetok->detail.pipe.open_prompt = 0;
	return (pipetok);
}

t_token	*init_redirection_details(t_token *redirectiontoken)
{
	redirectiontoken->detail.rdrc.fdin = -1;
	redirectiontoken->detail.rdrc.fdout = -1;
	redirectiontoken->detail.rdrc.rdrctype = NULL;
	return (redirectiontoken);
}

void	init_check_rdrc(t_list *last, char *last_type, char *last_str)
{
	last = NULL;
	last_type = NULL;
	last_str = NULL;
}

void	set_rdrctype(t_list *last, t_list *current, t_token *token)
{
	check_semantics(last, current);
	if (ft_strcmp(token->type, "Redirection") == 0)
	{
		if (ft_strcmp(token->str, "<<") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("here_doc");
		if (ft_strcmp(token->str, ">>") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("append");
		if (ft_strcmp(token->str, "<") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("redirection");
		if (ft_strcmp(token->str, ">") == 0)
			token->detail.rdrc.rdrctype = ft_strdup("truncate");
	}
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

int handle_redirections(t_minishell *m, t_list *ref, t_list *seq)
{
    t_token *token;
    char *file_name = NULL;

    while (seq != NULL) {
        token = (t_token *)seq->content;
        if (token->token == REDIRECTION)
        {
            seq = seq->next;
            if (seq == NULL || ((t_token *)seq->content)->token != WORD)
            {
                ft_fprintf(2, R"Error: Expected filename after redirection\n"D);
                return -1;
            }
            file_name = ((t_token *)seq->content)->str;

            if (ft_strcmp(token->detail.rdrc.rdrctype, "truncate") == 0 ||
                ft_strcmp(token->detail.rdrc.rdrctype, "append") == 0)
            {
                if (handle_output_redirection(m, token, file_name) == -1)
                    return -1;
            }
            else if (ft_strcmp(token->detail.rdrc.rdrctype, "redirection") == 0)
            {
                if (handle_input_redirection(m, file_name) == -1)
                    return -1;
            }
            else if (ft_strcmp(token->detail.rdrc.rdrctype, "here_doc") == 0)
            {
                if (handle_heredoc(m, token) == -1)
                    return -1;
            }
        }
        seq = seq->next;
    }
    return 0;
}

char	*prepare_executable_and_message(t_minishell *m, char *command)
{
	char	*executable;
	char	*mes;

	mes = ft_strjoin(ft_strjoin("bash: ", command), ": command not found");
	if (access(command, X_OK) == 0)
		executable = command;
	else
		executable = get_executable(m, command);
	if (!executable)
	{
		free(mes);
		return (NULL);
	}
	return (executable);
}

void	execute_command(t_minishell *m, char *executable, char **argv)
{
	int	pid;
	int	status;

	pid = fork();
	signal(SIGINT, handle_child_process);
	signal(SIGQUIT, handle_child_process);
	if (pid == -1)
		return (free(executable), pic_err(m, 1, "Fork failed"));
	else if (pid == 0)
	{
		execve(executable, argv, own_env(m->env_list));
		perror("execve failed");
		free(executable);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		m->exitcode = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		m->exitcode = 128 + WTERMSIG(status);
	signal(SIGINT, handle_main_process);
	signal(SIGQUIT, handle_main_process);
	free(executable);
}

void	execute(t_minishell *m)
{
	int		pipe_fd[2];
	int		prev_pipe_read;
	int		pid;
	int		status;
	int		i;
	t_list	*seq;

	prev_pipe_read = -1;
	i = 0;
	m->pipes = 0;
	pre_exec_prep(m);
	pre_exec_checks(m);
	if (m->leave)
		return ;
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
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			m->exitcode = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			m->exitcode = 128 + WTERMSIG(status);
	}
}
