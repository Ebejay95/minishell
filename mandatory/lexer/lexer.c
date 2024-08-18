/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 20:49:27 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void    add_token_to_list(t_list **lst, t_token *token)
{
    ft_lstadd_back(lst, ft_lstnew(token));
}

// void	prompt_to_token(char *prompt, t_list **tok_lst)
// {
// 	int		current_pos;
// 	int		quote_level;
// 	int		escape_next;
// 	int		current_token_size;
// 	t_token	*token;
// 	char	*ptr;
// 	char	*current_token;
// 	char	*expmap;
// 	char	*new_token;
// 	char	*new_expmap;
// 	int		new_size;

// 	current_pos = 0;
// 	quote_level = 0;
// 	escape_next = 0;
// 	current_token_size = 10;
// 	ptr = prompt;
// 	current_token = ft_calloc(current_token_size, sizeof(char));
// 	expmap = ft_calloc(current_token_size, sizeof(char));
// 	if (!current_token || !expmap)
// 	{
// 		free(current_token);
// 		free(expmap);
// 		ft_error_exit("malloc");
// 	}
// 	while (*ptr)
// 	{
// 		if (escape_next)
// 		{
// 			current_token[current_pos] = *ptr;
// 			if (quote_level == 2)
// 				expmap[current_pos] = '2';
// 			else
// 				expmap[current_pos] = '0';
// 			current_pos++;
// 			escape_next = 0;
// 			ptr++;
// 			continue ;
// 		}
// 		if (*ptr == '\\' && !escape_next)
// 		{
// 			escape_next = 1;
// 			current_token[current_pos] = *ptr;
// 			expmap[current_pos] = '0';
// 			current_pos++;
// 			ptr++;
// 			continue ;
// 		}
// 		if (*ptr == '\'' && quote_level == 0)
// 		{
// 			quote_level = 1;
// 			current_token[current_pos] = *ptr;
// 			expmap[current_pos] = 'S';
// 			current_pos++;
// 			ptr++;
// 			continue ;
// 		}
// 		if (*ptr == '\'' && quote_level == 1)
// 		{
// 			quote_level = 0;
// 			current_token[current_pos] = *ptr;
// 			expmap[current_pos] = 'S';
// 			current_pos++;
// 			ptr++;
// 			continue ;
// 		}
// 		if (*ptr == '"' && quote_level != 1)
// 		{
// 			current_token[current_pos] = *ptr;
// 			expmap[current_pos] = '0';
// 			current_pos++;
// 			if (quote_level == 2)
// 				quote_level = 0;
// 			else
// 				quote_level = 2;
// 			ptr++;
// 			continue ;
// 		}
// 		if (ft_isspace(*ptr) && quote_level == 0 && !escape_next)
// 		{
// 			if (current_pos > 0)
// 			{
// 				current_token[current_pos] = '\0';
// 				expmap[current_pos] = '\0';
// 				token = create_token(current_token, expmap);
// 				update_tok_type(token, WORD);
// 				add_token_to_list(tok_lst, token);
// 				current_pos = 0;
// 			}
// 			ptr++;
// 			continue ;
// 		}
// 		if (*ptr == '|' && quote_level == 0)
// 		{
// 			if (current_pos > 0)
// 			{
// 				current_token[current_pos] = '\0';
// 				expmap[current_pos] = '\0';
// 				token = create_token(current_token, expmap);
// 				update_tok_type(token, WORD);
// 				add_token_to_list(tok_lst, token);
// 				current_pos = 0;
// 			}
// 			token = create_token("|", "0");
// 			update_tok_type(token, PIPE);
// 			add_token_to_list(tok_lst, token);
// 			ptr++;
// 			continue ;
// 		}
// 		if ((*ptr == '>' || *ptr == '<') && quote_level == 0)
// 		{
// 			if (current_pos > 0)
// 			{
// 				current_token[current_pos] = '\0';
// 				expmap[current_pos] = '\0';
// 				token = create_token(current_token, expmap);
// 				update_tok_type(token, WORD);
// 				add_token_to_list(tok_lst, token);
// 				current_pos = 0;
// 			}
// 			if (*(ptr + 1) == *ptr)
// 			{
// 				if (*ptr == '>')
// 					token = create_token(">>", "00");
// 				else
// 					token = create_token("<<", "00");
// 				ptr++;
// 			}
// 			else
// 			{
// 				if (*ptr == '>')
// 					token = create_token(">", "0");
// 				else
// 					token = create_token("<", "0");
// 			}
// 			update_tok_type(token, REDIRECTION);
// 			add_token_to_list(tok_lst, token);
// 			ptr++;
// 			continue ;
// 		}
// 		current_token[current_pos] = *ptr;
// 		if (quote_level == 1)
// 			expmap[current_pos] = '1';
// 		else if (quote_level == 2)
// 			expmap[current_pos] = '2';
// 		else
// 			expmap[current_pos] = '0';
// 		current_pos++;
// 		if (current_pos >= current_token_size - 1)
// 		{
// 			new_size = current_token_size * 2;
// 			new_token = ft_calloc(new_size, sizeof(char));
// 			new_expmap = ft_calloc(new_size, sizeof(char));
// 			if (!new_token || !new_expmap)
// 			{
// 				free(current_token);
// 				free(expmap);
// 				if (new_token)
// 					free(new_token);
// 				if (new_expmap)
// 					free(new_expmap);
// 				ft_error_exit("calloc");
// 			}
// 			ft_memcpy(new_token, current_token, current_token_size);
// 			ft_memcpy(new_expmap, expmap, current_token_size);
// 			free(current_token);
// 			free(expmap);
// 			current_token = new_token;
// 			expmap = new_expmap;
// 			current_token_size = new_size;
// 		}
// 		ptr++;
// 	}
// 	if (current_pos > 0)
// 	{
// 		current_token[current_pos] = '\0';
// 		expmap[current_pos] = '\0';
// 		token = create_token(current_token, expmap);
// 		update_tok_type(token, WORD);
// 		add_token_to_list(tok_lst, token);
// 	}
// 	free(current_token);
// 	free(expmap);
// }
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


void	lex_prompt(t_minishell *m)
{
	// char	*tmpp;

	// tmpp = remove_chars(m->prompt, "\n");
	// m->prompt = tmpp;
	// if (tmpp != m->prompt)
	// {
	// 	free(m->prompt);
	// 	m->prompt = tmpp;
	// }
	detect_lexing_errors(m);
	prompt_to_token(m);
	if (DEBUG == 1)
	{
		ft_printf(Y"TOKENs:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	expand_toklst(m, &(m->tok_lst));
	if (DEBUG == 1)
	{
		ft_printf(Y"TOKENLIST:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	afterbreakup(&(m->tok_lst));
}
