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

t_token *newtoken_dup(t_token *src)
{
    if (!src)
        return NULL;

    t_token *dup = malloc(sizeof(t_token));
    if (!dup)
        return NULL;

    // Copy basic types
    dup->token = src->token;
    dup->had_quote = src->had_quote;
    dup->is_freed = src->is_freed;
    dup->open_prompt = src->open_prompt;
    dup->fd = src->fd;

    // Duplicate strings, handling NULL cases
    dup->str = src->str ? strdup(src->str) : NULL;
    dup->expmap = src->expmap ? strdup(src->expmap) : NULL;
    dup->rdrcmeta = src->rdrcmeta ? strdup(src->rdrcmeta) : NULL;
    dup->rdrctarget = src->rdrctarget ? strdup(src->rdrctarget) : NULL;

    // Check if any string duplication failed
    if ((src->str && !dup->str) ||
        (src->expmap && !dup->expmap) || (src->rdrcmeta && !dup->rdrcmeta) ||
        (src->rdrctarget && !dup->rdrctarget))
    {
        // Free any successfully allocated strings
        free(dup->str);
        free(dup->expmap);
        free(dup->rdrcmeta);
        free(dup->rdrctarget);
        free(dup);
        return NULL;
    }

    return dup;
}

// Function to free a token
void newfree_token(t_token *token)
{
    if (token)
    {
        free(token->str);
        free(token->expmap);
        free(token->rdrcmeta);
        free(token->rdrctarget);
        free(token);
    }
}
void split_pipes(t_minishell *m)
{
    t_list *current;
    t_list *temp_list = NULL;
    int pipe_index = 0;

    // Initialize cmd_seqs with NULL
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
            // Duplicate the token before adding it to the temporary list
            t_token *token_dup = newtoken_dup(token);
            if (!token_dup)
                ft_error_exit("token_dup");

            t_list *new_node = ft_lstnew(token_dup);
            if (!new_node)
            {
                free(token_dup);  // Free the duplicated token if node creation fails
                ft_error_exit("ft_lstnew");
            }
            ft_lstadd_back(&temp_list, new_node);
        }

        current = current->next;
    }

    // Add the last list if it exists
    if (temp_list != NULL)
    {
        m->cmd_seqs[pipe_index] = temp_list;
    }
}
