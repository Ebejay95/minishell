/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_runseg_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:31:50 by chorst            #+#    #+#             */
/*   Updated: 2024/08/16 01:40:27 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void add_argument(char ***args, int *arg_count, char *arg)
{
    // Wenn args NULL ist, wird Speicher für das erste Element allokiert
    if (*args == NULL)
    {
        *args = ft_calloc(2, sizeof(char *));
        (*args)[0] = arg;
        (*args)[1] = NULL; // Null-terminieren
        *arg_count = 1;
    }
    else
    {
        // Speicher für ein zusätzliches Argument allokieren
        char **temp = ft_calloc(*arg_count + 2, sizeof(char *));
        
        // Bestehende Argumente kopieren
        memcpy(temp, *args, *arg_count * sizeof(char *));
        
        // Neues Argument hinzufügen
        temp[*arg_count] = arg;
        temp[*arg_count + 1] = NULL; // Null-terminieren
        
        // Alten Speicher freigeben und args aktualisieren
        free(*args);
        *args = temp;
        
        // arg_count erhöhen
        (*arg_count)++;
    }
}

void	exec_builtin_cmd(t_minishell *m, char **args, int arg_count, t_fd *fd)
{
	if (fd->last_input != fd->input)
		dup2(fd->last_input, STDIN_FILENO);
	if (fd->last_output != fd->output)
		dup2(fd->last_output, STDOUT_FILENO);
	execute_builtin(m, args[0], args, arg_count);
}

void execute_external_command(t_minishell *m, char **args, t_fd *fd)
{
    pid_t pid;
    char *tmp;

    tmp = get_executable(m, args[0]);
    if (!tmp)
    {
        ft_fprintf(2, "bash: %s: Command not found\n", args[0]);
        m->exitcode = 127;
        return;
    }
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    pid = fork();
    if (pid == 0)
    {
        setup_child_process(fd);
        execve(tmp, args, m->envp);  // Direkte Ausführung des Befehls
        ft_fprintf(2, "execve failed\n");
        exit(1);
    }
    else if (pid < 0)
    {
        ft_fprintf(2, "Fork failed\n");
        m->exitcode = 1;
    }
    else
        run_parent_process(m, pid);
    free(tmp);
}

void process_tok(t_list *exec_lst, t_fd *fd, char ***args, int *arg_count)
{
    t_list  *current;
    t_token *token;
    char    *dup_str;

    current = exec_lst;
    while (current != NULL)
    {
        token = (t_token *)current->content;
        if (token->token == REDIRECTION)
        {
            run_redirection(token, fd);
        }
        else if (token->token == COMMAND || token->token == WORD)
        {
            dup_str = ft_strdup(token->str);
            if (!dup_str)
            {
                ft_fprintf(2, "Error: Memory allocation failed for token string duplication\n");
                cleanup_args(*args, *arg_count);
                return;
            }
            add_argument(args, arg_count, dup_str);
        }
        current = current->next;
    }
}