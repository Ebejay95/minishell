/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_command_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:08:29 by chorst            #+#    #+#             */
/*   Updated: 2024/08/07 10:41:49 by chorst           ###   ########.fr       */
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
