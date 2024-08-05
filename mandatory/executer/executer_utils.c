/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:58:30 by chorst            #+#    #+#             */
/*   Updated: 2024/08/05 15:19:13 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

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
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (pic_err(m, 1, "Fork failed"), free(executable));
	else if (pid == 0)
	{
		execve(executable, argv, own_env(m->env_list));
		perror("execve failed");
		free(executable);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			m->exitcode = WEXITSTATUS(status);
	}
	free(executable);
}
