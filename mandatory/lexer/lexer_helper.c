/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:54:56 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 18:59:40 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

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

//free(cur_content->expmap);
//cur_content->expmap = NULL;
//ft_printf(R"%s\n"D, cur_content->str);
//ft_printf(G"%s\n"D, cur_content->expmap);
void	expand_toklst(t_minishell *m, t_list **tok_lst)
{
	t_list	*current;
	t_token	*cur_content;

	current = *tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		expand_token(m, cur_content);
		current = current->next;
	}
}

//if (DEBUG == 1)
//{
//	ft_printf(Y"TOKENLIST:\n"D);
//	ft_lstput(&(m->tok_lst), put_token, '\n');
//}
void	lex_prompt(t_minishell *m)
{
	char	*tmpp;

	printf("i was here\n");
	tmpp = remove_chars(m->prompt, "\n");
	return ;
	m->prompt = tmpp;
	if (tmpp != m->prompt)
	{
		free(m->prompt);
		m->prompt = tmpp;
	}
	detect_lexing_errors(m);
	prompt_to_token(m->prompt, &(m->tok_lst));
	expand_toklst(m, &(m->tok_lst));
	afterbreakup(&(m->tok_lst));
}
