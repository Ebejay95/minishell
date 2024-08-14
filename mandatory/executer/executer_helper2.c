/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:16:46 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 12:32:24 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static char	*find_executable(char **paths, int pathcount, char *command)
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

void	reset_sequences(t_minishell *m)
{
	int	i;

	if (m->exec_seqs)
	{
		i = 0;
		while (m->exec_seqs[i])
		{
			ft_lstclear(&(m->exec_seqs[i]), free_token);
			i++;
		}
		free(m->exec_seqs);
		m->exec_seqs = NULL;
	}
	if (m->exec_lst)
	{
		ft_lstclear(&(m->exec_lst), NULL);
		m->exec_lst = NULL;
	}
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
		result = find_executable(paths, pathcount, command);
		ft_array_l_free(paths, pathcount);
		return (result);
	}
	return (NULL);
}

void	execute_command(t_minishell *m, char *executable, char **argv)
{
	char	**own;

	own = own_env(m->env_list);
	if (is_builtin(executable))
		execute_builtin(m, executable, argv, ft_array_length(argv));
	else
	{
		execve(executable, argv, own);
		perror("execve failed");
		free(executable);
		exit(EXIT_FAILURE);
	}
}

int	keep_for_exec(t_token *token)
{
	if (token->token == WORD || token->token == COMMAND || token->token == PIPE)
		return (1);
	return (0);
}