/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:08:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 09:56:19 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function to free a token
void split_pipes(t_minishell *m)
{
    t_list *current;
    t_list *temp_list = NULL;
    int pipe_index = 0;

    // Initialisiere cmd_seqs mit NULL
    for (int i = 0; i < 1024; i++)
    {
        m->cmd_seqs[i] = NULL;
    }

    current = m->tok_lst;
    m->pipes = 0;

    while (current != NULL)
    {
        t_token *token = (t_token *)current->content;

        if (token->token == PIPE)
        {
            if (temp_list != NULL)
            {
                m->cmd_seqs[pipe_index] = temp_list;
                temp_list = NULL;
                pipe_index++;
            }
            m->pipes++;
        }
        else
        {
            // Duplizieren des Tokens vor dem Hinzufügen zur temporären Liste
            t_token *token_dup = duplicate(token);
            if (!token_dup)
                ft_error_exit("token_dup");

            t_list *new_node = ft_lstnew(token_dup);
            if (!new_node)
            {
                // Falls Listenknoten-Erstellung fehlschlägt
                newfree_token(token_dup);  // Freigeben des duplizierten Tokens
                ft_error_exit("ft_lstnew");
            }
            ft_lstadd_back(&temp_list, new_node);
        }

        current = current->next;
    }

    // Hinzufügen der letzten Liste, falls vorhanden
    if (temp_list != NULL)
    {
        m->cmd_seqs[pipe_index] = temp_list;
    }
}
