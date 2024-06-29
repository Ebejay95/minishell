/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/29 02:17:48 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	add_token_to_list(t_list **lst, t_token *token)
{
	t_list	*new_element;
	t_list	*current;

	new_element = ft_lstnew((void *)token);
	if (!new_element)
		return ;
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

char	*remove_backslashes(char *str)
{
	int		count;
	char	*temp;
	char	*new_str;
	char	*new_str_ptr;

	count = 0;
	temp = str;
	while (*temp)
	{
		if (*temp != '\\')
			count++;
		temp++;
	}
	new_str = (char *) malloc((count + 1) * sizeof(char));
	if (new_str == NULL)
		exit(EXIT_FAILURE);
	new_str_ptr = new_str;
	while (*str)
	{
		if (*str != '\\')
		{
			*new_str_ptr = *str;
			new_str_ptr++;
		}
		str++;
	}
	*new_str_ptr = '\0';
	return (new_str);
}

void	prompt_to_token(t_minishell *m)
{
	t_token	*token;
	int		is_single;
	int		is_double;
	int		start_pos;
	int		current_pos;
	char	*token_str;
	char	*work;
	char	quote_type;

	work = m->prompt;
	is_single = 0;
	is_double = 0;
	start_pos = 0;
	current_pos = 0;
	quote_type = 0;
	while (work[current_pos])
	{
		if (work[current_pos] == '\\' && (work[current_pos + 1] == '\'' || work[current_pos + 1] == '"' || work[current_pos + 1] == '\\'))
			current_pos++;
		else if (work[current_pos] == '\'' && !is_double)
		{
			is_single = !is_single;
			if (is_single && quote_type == 0)
				quote_type = '\'';
		}
		else if (work[current_pos] == '"' && !is_single)
		{
			is_double = !is_double;
			if (is_double && quote_type == 0)
				quote_type = '"';
		}
		else if (ft_isspace(work[current_pos]) && !is_single && !is_double)
		{
			if (current_pos - start_pos > 0)
			{
				token_str = ft_strndup(work + start_pos, current_pos - start_pos);
				if (token_str)
				{
					token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos, quote_type != '\'');
					add_token_to_list(&m->tok_lst, token);
				}
				quote_type = 0;
			}
			start_pos = current_pos + 1;
		}
		current_pos++;
	}
	if (current_pos - start_pos > 0)
	{
		token_str = ft_strndup(work + start_pos, current_pos - start_pos);
		if (token_str)
		{
			token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos, quote_type != '\'');
			add_token_to_list(&m->tok_lst, token);
		}
	}
	if (is_single || is_double)
		ft_printf("Error: Mismatched quotes in input.\n");
}

void	analyze_token(t_token *token)
{
	update_tok_type(token, ARGUMENT);
}

void	analyze_tokens(t_minishell *m)
{
	t_list		*current;

	current = m->tok_lst;
	while (current != NULL)
	{
		analyze_token((t_token *)current->content);
		current = current->next;
	}
}

void	lex_prompt(t_minishell *m)
{
	prompt_to_token(m);
	analyze_tokens(m);
	ft_printf(Y"TOKENLIST:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
}

// t_segment	*find_seg_by_start(t_segment **segs, int search)
// {
// 	int	segdex;

// 	segdex = 0;
// 	while (segs[segdex])
// 	{
// 		if (segs[segdex]->start == search)
// 			return (segs[segdex]);
// 		segdex++;
// 	}
// 	return (NULL);
// }

// t_segment	*find_seg_by_end(t_segment **segs, int search)
// {
// 	int	segdex;

// 	segdex = 0;
// 	while (segs[segdex])
// 	{
// 		if (segs[segdex]->end == search)
// 			return (segs[segdex]);
// 		segdex++;
// 	}
// 	return (NULL);
// }

// t_segment	*find_seg_by_str(t_segment **segs, char *search)
// {
// 	int	segdex;

// 	segdex = 0;
// 	while (segs[segdex])
// 	{
// 		if (ft_strcmp(segs[segdex]->str, search) == 0)
// 			return (segs[segdex]);
// 		segdex++;
// 	}
// 	return (NULL);
// }
