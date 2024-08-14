/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 15:00:39 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	breakup(t_list **current, t_token *cur, int *i)
{
	char	*work;
	char	**words;
	int		wordcount;
	t_token	*new_token;
	t_list	*new_node;

	work = whitespace_handler(cur->str);
	wordcount = ft_count_words(work, ' ');
	words = ft_split(work, ' ');
	free(cur->str);
	cur->str = words[0];
	while (*i < wordcount)
	{
		new_token = create_token(words[*i], cur->expmap);
		update_tok_type(new_token, WORD);
		new_node = ft_lstnew(new_token);
		new_node->next = (*current)->next;
		(*current)->next = new_node;
		*current = new_node;
		(*i)++;
	}
	free(words);
	free(work);
}

void	afterbreakup(t_list **tok_lst)
{
	t_list	*current;
	t_token	*cur;
	int		i;

	i = 1;
	current = *tok_lst;
	while (current != NULL)
	{
		cur = (t_token *)current->content;
		if (!ft_strcontains(cur->expmap, 'X'))
		{
			if (ft_strcontains(cur->expmap, 'E'))
				breakup(&current, cur, &i);
			else
				current = current->next;
		}
		else
			current = current->next;
	}
}

void	detect_lexing_errors(t_minishell *m)
{
	char	*work;
	char	quote_char;
	int		in_quote;

	work = m->prompt;
	quote_char = 0;
	in_quote = 0;
	while (*work != '\0')
	{
		if (!in_quote && (*work == '\'' || *work == '\"'))
		{
			quote_char = *work;
			in_quote = 1;
		}
		else if (in_quote && *work == quote_char)
			in_quote = 0;
		work++;
	}
	if (in_quote)
		pic_err(m, 2, "unclosed quotes");
}

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

void	lex_prompt(t_minishell *m)
{
	char	*tmpp;

	tmpp = remove_chars(m->prompt, "\n");
	m->prompt = tmpp;
	if (tmpp != m->prompt)
	{
		free(m->prompt);
		m->prompt = tmpp;
	}
	detect_lexing_errors(m);
	prompt_to_token(m->prompt, &(m->tok_lst));
	expand_toklst(m, &(m->tok_lst));
	if (DEBUG == 1)
	{
		ft_printf(Y"TOKENLIST:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	afterbreakup(&(m->tok_lst));
}
