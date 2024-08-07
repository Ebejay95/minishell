/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:29:07 by chorst            #+#    #+#             */
/*   Updated: 2024/08/07 11:46:40 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

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
