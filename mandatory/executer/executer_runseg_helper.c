/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:31:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 14:35:43 by chorst           ###   ########.fr       */
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
