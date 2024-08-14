/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:29:07 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 14:41:08 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

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
