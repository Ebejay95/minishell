/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:11:38 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 19:40:39 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	ft_error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	pic_err(t_minishell *m, int code, char *mes)
{
	ft_fprintf(2, "%s\n", mes);
	m->leave = 1;
	m->exitcode = code;
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

int	add_arg(char ***argv, int *capacity, t_list *temp, int *argc)
{
	t_token	*token;

	if (*argc >= *capacity)
		if (!resize_argv(argv, capacity))
			return (0);
	token = (t_token *)temp->content;
	(*argv)[*argc] = ft_strdup(token->str);
	if (!(*argv)[*argc])
	{
		cleanup(*argv);
		return (0);
	}
	(*argc)++;
	return (1);
}

int	add_first_arg(char **argv, t_list *current, int *argc)
{
	t_token	*token;

	token = (t_token *)current->content;
	argv[0] = ft_strdup(token->str);
	if (!argv[0])
	{
		free(argv);
		return (0);
	}
	*argc = 1;
	return (1);
}
