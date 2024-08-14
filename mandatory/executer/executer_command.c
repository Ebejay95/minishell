/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:29:07 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 19:26:05 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static char	*prepare_executable_and_message(t_minishell *m, char *command)
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

static void	ft_run_others(t_minishell *m, char *command, char **argv)
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

static void	pic_var_or_others(t_minishell *m, char *com, char **argv, int argc)
{
	if (!is_var_name(m->env_list, &argv[0]))
		ft_export(argc, argv, &m->env_list);
	else
		ft_run_others(m, com, argv);
}

static char	**prepare_argv(t_list *current, int *argc)
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

void	run_command(t_minishell *m, t_list *current)
{
	t_token	*token;
	char	*command;
	char	**argv;
	int		argc;

	if (current == NULL)
		return ;
	token = (t_token *)current->content;
	while (token->token != COMMAND)
	{
		current = current->next;
		token = (t_token *)current->content;
	}
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
