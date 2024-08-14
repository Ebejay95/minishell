/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 23:19:21 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

static void add_seq_to_cmd_seq(t_list ***cmd_seq, t_list *seq, int *count)
{
    t_list **new_cmd_seq;

    new_cmd_seq = ft_calloc(*count + 2, sizeof(t_list *));
    if (!new_cmd_seq)
        ft_error_exit("ft_calloc");

    if (*cmd_seq)
    {
        for (int i = 0; i < *count; i++)
            new_cmd_seq[i] = (*cmd_seq)[i];
        free(*cmd_seq);
    }

    new_cmd_seq[*count] = seq;
    (*count)++;
    *cmd_seq = new_cmd_seq;
}

void split_pipes(t_minishell *m, t_list ***cmd_seq, t_list ***exec_seq)
{
    int count = 0;
    t_list *current = m->tok_lst;
    t_list *seq = NULL;
    t_token *token;

    *cmd_seq = NULL;
    m->pipes = 0;

    while (current != NULL)
    {
        token = (t_token *)current->content;
        if (token->token == PIPE)
        {
            if (seq != NULL)
            {
                add_seq_to_cmd_seq(cmd_seq, seq, &count);
                seq = NULL;
            }
            m->pipes++;
        }
        else
        {
            if (seq == NULL || (token->token == COMMAND && seq->content != NULL))
            {
                if (seq != NULL)
                    add_seq_to_cmd_seq(cmd_seq, seq, &count);
                seq = NULL;
            }
            add_token_to_list(&seq, token);
        }
        current = current->next;
    }

    if (seq != NULL)
    {
        add_seq_to_cmd_seq(cmd_seq, seq, &count);
    }

    // Allocate exec_seq
    *exec_seq = ft_calloc(count + 1, sizeof(t_list *));
    if (!*exec_seq)
        ft_error_exit("ft_calloc");

    // Debug output
    if (DEBUG == 1)
    {
        ft_printf("split_pipes result:\n");
        for (int i = 0; i < count; i++)
        {
            ft_printf("Command sequence %d:\n", i);
            ft_lstput(&((*cmd_seq)[i]), put_token, '\n');
        }
    }
}
