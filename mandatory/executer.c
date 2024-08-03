/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/03 07:34:02 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

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
				break;
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
		// Kindprozess
		execve(executable, argv, own_env(m->env_list));
		perror("execve failed");
		free(executable);
		free(message);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Elternprozess
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			m->exitcode = WEXITSTATUS(status);
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
		return ((void)ft_printf("No command to run.\n"));
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
	// Allocate space for argv, including space for the command and NULL terminator
	argv = (char **)malloc((argc + 2) * sizeof(char *)); // +1 for command, +1 for NULL
	if (!argv)
		return (perror("malloc failed"));
	// Set the command as the first element of argv
	argv[i++] = ft_strdup(command);
	if (!argv[0])
		return (perror("ft_strdup failed"), free(argv));
	noword = 0;
	// Add the rest of the arguments to argv
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
	argv[i] = NULL; // Null-terminate the argv array
	ft_printf("execute %s\n", command);
	// Execute the command with arguments
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
	char	*last_str;

	last_type = NULL;
	last_str = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0 && last_type == NULL)
			handle_error(m, 2, "bash: syntax error near unexpected token `|'");
		else if (current->next == NULL && !ft_strcmp(cur_content->type, "Pipe"))
			cur_content->detail.pipe.open_prompt = 1;
		last_type = cur_content->type;
		last_str = cur_content->str;
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
// |---------------------------------------------------------------------------|
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

int	pre_exec_checks(t_minishell *m)
{
	if (m->exitcode == 0)
		check_pipes(m);
	if (m->exitcode == 0)
		check_redirections(m);
	return (m->exitcode);
}

void execute(t_minishell *m)
{
	//int		pipe_fd[2];
	//int		pid;
	int		prev_pipe_read;
	t_list	*current;
	t_token	*token;
	//int		status;

	pre_exec_prep(m);
	if (m->exitcode == 0)
		m->exitcode = pre_exec_checks(m);
	if (m->exitcode == 0)
	{
		ft_printf(Y"EXECUTE:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	if (m->exitcode != 0)
		return;
	prev_pipe_read = STDIN_FILENO;
	current = m->tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == COMMAND)
		{
			run_command(m, current);
		}
		// if (token->token == PIPE)
		// {
		// 	if (pipe(pipe_fd) == -1)
		// 		handle_error(m, 2, "Pipe creation failed");
		// 	pid = fork();
		// 	if (pid == -1)
		// 		handle_error(m, 2, "Fork failed");
		// 	else if (pid == 0)
		// 	{
		// 		close(pipe_fd[0]);
		// 		dup2(prev_pipe_read, STDIN_FILENO);
		// 		dup2(pipe_fd[1], STDOUT_FILENO);
		// 		close(pipe_fd[1]);
		// 		run_command(m, current);
		// 		exit(EXIT_SUCCESS);
		// 	}
		// 	else
		// 	{
		// 		close(pipe_fd[1]);
		// 		if (prev_pipe_read != STDIN_FILENO)
		// 			close(prev_pipe_read);
		// 		prev_pipe_read = pipe_fd[0];
		// 	}
		// }
		// else if (token->token == COMMAND)
		// {
		// 	pid = fork();
		// 	if (pid == -1)
		// 		handle_error(m, 2, "Fork failed");
		// 	else if (pid == 0)
		// 	{
		// 		if (prev_pipe_read != STDIN_FILENO)
		// 		{
		// 			dup2(prev_pipe_read, STDIN_FILENO);
		// 			close(prev_pipe_read);
		// 		}
		// 		run_command(m, current);
		// 		exit(EXIT_SUCCESS);
		// 	}
		// }
		current = current->next;
	}
	// while (wait(&status) > 0)
	// {
	// 	if (WIFEXITED(status))
	// 		m->exitcode = WEXITSTATUS(status);
	// }
	// if (prev_pipe_read != STDIN_FILENO)
	// 	close(prev_pipe_read);
}
