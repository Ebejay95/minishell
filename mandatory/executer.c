/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/01 17:57:23 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	handle_error(t_minishell *m, int code, char *message)
{
	ft_fprintf(2, R"%s\n"D, message);
	m->exitcode = code;
}

char	*get_executable(t_minishell *m, char *command)
{
	char	*pathline;
	char	*joined;
	char	**paths;
	int		pathcount;
	int		i;

	pathline = my_getenv("PATH", m->env_list);
	pathcount = ft_count_words(pathline, ':');
	paths = ft_split(pathline, ':');
	if (pathline != NULL)
	{
		i = 0;
		while (i < pathcount)
		{
			joined = ft_strjoin(paths[i], command);
			if (joined == NULL)
				break ;
			if (access(joined, X_OK) == 0)
				return (ft_array_l_free(paths, pathcount), joined);
			i++;
			free(joined);
		}
	}
	ft_array_l_free(paths, pathcount);
	return (NULL);
}

char	*ft_cmdnf(char * command)
{
	char	*bash;
	char	*nofound;
	char	*pre;
	char	*message;

	bash = ft_strdup("bash: ");
	nofound = ft_strdup(": command not found");
	pre = ft_strjoin(bash, command);
	message = ft_strjoin(pre, nofound);
	return (message);
}

char	*join_env_var(char *key, char *value)
{
	char	*pre;
	char	*joint;

	pre = ft_strjoin(key, "=");
	joint = ft_strjoin(pre, value);
	return (joint);
}

char **own_env(t_envlst *env_list) {
    t_envlst *temp = env_list;
    size_t count = 0;

    while (temp != NULL) {
        if (temp->value != NULL) {
            count++;
        }
        temp = temp->next;
    }

    char **env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array) {
        return NULL;
    }

    temp = env_list;
    size_t index = 0;
    while (temp != NULL) {
        if (temp->value != NULL) {
            env_array[index] = join_env_var(temp->name, temp->value);
            if (!env_array[index]) {
                for (size_t i = 0; i < index; i++) {
                    free(env_array[i]);
                }
                free(env_array);
                return NULL;
            }
            index++;
        }
        temp = temp->next;
    }
    env_array[index] = NULL;

    return env_array;
}

void	ft_run_others(t_minishell *m, char *command, char **argv)
{
	char	*rel_command;
	char	*executable;
	char	*message;

	message = ft_cmdnf(command);
	rel_command = ft_strjoin("/", command);
	executable = get_executable(m, rel_command);
	if (!executable)
		handle_error(m, 0, message); /// TODO fuer JOnathan Hnadle das korrekt!!!! ich habe pronbleme mit dem error code
	execve(executable, argv, own_env(m->env_list));// Warum acuh immer werden wir hier rausgeschmissen
}

void run_command(t_minishell *m, t_list *current)
{
    t_token *token;
    char **argv;
    char *command;
    t_envlst **envlist;
    int argc = 0;
    int i = 0;
    int noword = 0;
    t_list *arg_list;

    if (current == NULL)
    {
        ft_printf("No command to run.\n");
        return;
    }
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
        {
            argc++;
        }
		else
			noword = 1;
        arg_list = arg_list->next;
    }

    // Allocate space for argv, including space for the command and NULL terminator
    argv = (char **)malloc((argc + 2) * sizeof(char *)); // +1 for command, +1 for NULL
    if (!argv)
    {
        perror("malloc failed");
        return;
    }

    // Set the command as the first element of argv
    argv[i++] = ft_strdup(command);
    if (!argv[0])
    {
        perror("ft_strdup failed");
        free(argv);
        return;
    }

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
                {
                    free(argv[--i]);
                }
                free(argv);
                return;
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
    else if (ft_strcmp(command, "echo") == 0)
        ft_echo(argv);
    else if (ft_strcmp(command, "env") == 0)
        ft_env(*envlist);
    else if (ft_strcmp(command, "exit") == 0)
        ft_exit(argv);
    else if (ft_strcmp(command, "export") == 0 && is_var_name(*envlist, &argv[0])) // TODO !!!!! is_var_name
        ft_export(i, argv, &envlist);
    else if (ft_strcmp(command, "pwd") == 0)
        ft_pwd(argv);
    else if (ft_strcmp(command, "unset") == 0)
        ft_unset(envlist, argv);
    else
		ft_run_others(m, command, argv);

    i = 0;
    while (argv[i] != NULL)
    {
        free(argv[i]);
        i++;
    }
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
		if (ft_strcmp(token->type, "Word") == 0 && ft_strcmp(last_token->type, "Redirection") == 0 && ft_strcmp(last_token->str, "<<") == 0)
		{
			update_tok_type(token, DELIMITER);
		}
		if (ft_strcmp(token->type, "Word") == 0 && ft_strcmp(last_token->type, "Pipe") == 0 && ft_strcmp(last_token->str, "|") == 0)
		{
			update_tok_type(token, COMMAND);
		}
	}
	else
	{
		if (ft_strcmp(token->type, "Word") == 0)
		{
			update_tok_type(token, COMMAND);
		}
	}
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
	redirectiontoken->detail.redirection.fdin = -1;
	redirectiontoken->detail.redirection.fdout = -1;
	redirectiontoken->detail.redirection.rdrctype = NULL;
	return (redirectiontoken);
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
		else if (current->next == NULL && ft_strcmp(cur_content->type, "Pipe") == 0)
		{
			cur_content->detail.pipe.open_prompt = 1;
		}
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
		{
			token->detail.redirection.rdrctype = ft_strdup("here_doc");
		}
		if (ft_strcmp(token->str, ">>") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("append");
		}
		if (ft_strcmp(token->str, "<") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("redirection");
		}
		if (ft_strcmp(token->str, ">") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("truncate");
		}
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

int	pre_exec_checks(t_minishell *m)
{
	if (m->exitcode == 0)
		check_pipes(m);
	if (m->exitcode == 0)
		check_redirections(m);
	return m->exitcode;
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
