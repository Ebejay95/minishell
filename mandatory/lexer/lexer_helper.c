/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:54:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 14:49:59 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	add_token_to_list(t_list **lst, t_token *token)
{
	t_list	*new_element;
	t_list	*current;

	if (token)
	{
		new_element = ft_lstnew((void *)token);
		if (!new_element)
		{
			free_token(token);
			return ;
		}
		if (*lst == NULL)
			*lst = new_element;
		else
		{
			current = *lst;
			while (current->next != NULL)
				current = current->next;
			current->next = new_element;
		}
	}
}

void	add_new_token(t_list **current, char *word, char *expmap)
{
	t_token	*new_token;
	t_list	*new_node;

	new_token = create_token(word, expmap);
	update_tok_type(new_token, WORD);
	new_node = ft_lstnew(new_token);
	new_node->next = (*current)->next;
	(*current)->next = new_node;
	*current = new_node;
}

void	pr_lextoken(t_list **current, t_token *cur)
{
	char	*work;
	char	**words;
	int		wordcount;
	int		i;

	work = whitespace_handler(cur->str);
	wordcount = ft_count_words(work, ' ');
	words = ft_split(work, ' ');
	free(cur->str);
	cur->str = words[0];
	i = 1;
	while (i < wordcount)
	{
		add_new_token(current, words[i], cur->expmap);
		i++;
	}
	free(words);
}

void	afterbreakup(t_list **tok_lst)
{
	t_list	*current;
	t_token	*c;

	current = *tok_lst;
	while (current != NULL)
	{
		c = (t_token *)current->content;
		if (!ft_strcontains(c->expmap, 'X') && ft_strcontains(c->expmap, 'E'))
		{
			pr_lextoken(&current, c);
		}
		current = current->next;
	}
}
