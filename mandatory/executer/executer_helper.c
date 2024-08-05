/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:11:38 by chorst            #+#    #+#             */
/*   Updated: 2024/08/05 17:53:07 by chorst           ###   ########.fr       */
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
	ft_fprintf(2, R"%s\n"D, mes);
	m->exitcode = code;
}

char	*text(int message)
{
	if (message == 1)
		return ("bash: syntax error near unexpected token `newline'");
	if (message == 2)
		return ("bash: syntax error near unexpected token `|'");
	if (message == 3)
		return ("bash: syntax error near unexpected token `>'");
	if (message == 4)
		return ("bash: syntax error near unexpected token `>>'");
	if (message == 5)
		return ("bash: syntax error near unexpected token `<'");
	if (message == 6)
		return ("bash: syntax error near unexpected token `<<'");
	if (message == 7)
		return ("Redirection");
	if (message == 8)
		return ("Pipe");
	if (message == 9)
		return ("Word");
	if (message == 10)
		return ("No file name specified for redirection\n");
	if (message == 11)
		return ("Failed to open file for writing (truncate/append)");
	if (message == 12)
		return ("Command");
	return (NULL);
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

void	ft_run_others(t_minishell *m, char *command, char **argv)
{
	char	*executable;
	char	*mes;

	executable = prepare_executable_and_message(m, command);
	if (!executable)
		return ;
	mes = ft_strjoin(ft_strjoin("bash: ", command), ": command not found");
	execute_command(m, executable, argv);
	free(mes);
}
