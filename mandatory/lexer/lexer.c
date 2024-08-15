/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 18:13:52 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

	//jonathaneberle@MAC42 minishell % ./minishell
	//🍕🚀🌈🦄🍺 /bin/echo '"$USER"'
	//TOKENLIST:
	//[Word$ /bin/echo$ map: 000000000]
	//[Word$ b$USERh$ map: 00]
	//execute:
	//[Command$ /bin/echo$ map: 000000000]
	//[Word$ b$USERh$ map: 00]
	//EXEC without pipe
	//cmd_seqs:
	//[Command$ /bin/echo$ map: 000000000]
	//[Word$ b$USERh$ map: 00]
	//exec_seqs:
	//b$USERh
	//🍕🚀🌈🦄🍺

void	add_new_abtoken(t_list **current, char *word, char *expmap)
{
	t_token	*new_token;
	t_list	*new_node;

	ft_printf("add_new_abtoken\n");
	new_token = create_token(word, expmap);
	update_tok_type(new_token, WORD);
	new_node = ft_lstnew(new_token);
	new_node->next = (*current)->next;
	(*current)->next = new_node;
	*current = new_node;
}

void	process_abtoken(t_list **current, t_token *cur)
{
	ft_printf("process_abtoken\n");
	char	*work;
	char	**words;
	int		wordcount;
	int		i;
	t_list	*next_node;

	next_node = (*current)->next;  // Speichern des nächsten Knotens
	work = whitespace_handler(cur->str);
	wordcount = ft_count_words(work, ' ');
	words = ft_split(work, ' ');
	free(cur->str);
	cur->str = words[0];
	i = 1;
	while (i < wordcount)
	{
		ft_printf("process_abtoken while\n");
		add_new_abtoken(current, words[i], cur->expmap);
		i++;
	}
	free(words);
	*current = next_node;  // Setzen des current auf den nächsten Knoten
}

int	should_process_token(t_token *token)
{
	return (!ft_strcontains(token->expmap, 'X') && ft_strcontains(token->expmap, 'E'));
}

void	afterbreakup(t_list **tok_lst)
{
	t_list	*current;
	t_token	*cur;

	ft_printf("afterbreakup\n");
	current = *tok_lst;
	while (current != NULL)
	{
		ft_printf("afterbreakup while\n");
		cur = (t_token *)current->content;
		if (should_process_token(cur))
		{
			process_abtoken(&current, cur);
		}
		else
		{
			current = current->next;
		}
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

void    add_token_to_list(t_list **lst, t_token *token)
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

void	prompt_to_token(char *prompt, t_list **tok_lst)
{
	int		current_pos;
	int		quote_level;
	int		escape_next;
	int		current_token_size;
	t_token	*token;
	char	*ptr;
	char	*current_token;
	char	*expmap;
	char	*new_token;
	char	*new_expmap;
	int		new_size;

	current_pos = 0;
	quote_level = 0;
	escape_next = 0;
	current_token_size = 10;
	ptr = prompt;
	current_token = ft_calloc(current_token_size, sizeof(char));
	expmap = ft_calloc(current_token_size, sizeof(char));
	if (!current_token || !expmap)
	{
		free(current_token);
		free(expmap);
		ft_error_exit("malloc");
	}
	while (*ptr)
	{
		if (escape_next)
		{
			current_token[current_pos] = *ptr;
			if (quote_level == 2)
				expmap[current_pos] = '2';
			else
				expmap[current_pos] = '0';
			current_pos++;
			escape_next = 0;
			ptr++;
			continue ;
		}
		if (*ptr == '\'' && quote_level == 0)
		{
			quote_level = 1;
			current_token[current_pos] = *ptr;
			expmap[current_pos] = 'S';
			current_pos++;
			ptr++;
			continue ;
		}
		if (*ptr == '\'' && quote_level == 1)
		{
			quote_level = 0;
			current_token[current_pos] = *ptr;
			expmap[current_pos] = 'S';
			current_pos++;
			ptr++;
			continue ;
		}
		if (*ptr == '"' && quote_level != 1)
		{
			current_token[current_pos] = *ptr;
			expmap[current_pos] = '0';
			current_pos++;
			if (quote_level == 2)
				quote_level = 0;
			else
				quote_level = 2;
			ptr++;
			continue ;
		}
		if (ft_isspace(*ptr) && quote_level == 0)
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(tok_lst, token);
				current_pos = 0;
			}
			ptr++;
			continue ;
		}
		if (*ptr == '|' && quote_level == 0)
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(tok_lst, token);
				current_pos = 0;
			}
			token = create_token("|", "0");
			update_tok_type(token, PIPE);
			add_token_to_list(tok_lst, token);
			ptr++;
			continue ;
		}
		if ((*ptr == '>' || *ptr == '<') && quote_level == 0)
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(tok_lst, token);
				current_pos = 0;
			}
			if (*(ptr + 1) == *ptr)
			{
				if (*ptr == '>')
					token = create_token(">>", "00");
				else
					token = create_token("<<", "00");
				ptr++;
			}
			else
			{
				if (*ptr == '>')
					token = create_token(">", "0");
				else
					token = create_token("<", "0");
			}
			update_tok_type(token, REDIRECTION);
			add_token_to_list(tok_lst, token);
			ptr++;
			continue ;
		}
		current_token[current_pos] = *ptr;
		if (quote_level == 1)
			expmap[current_pos] = '1';
		else if (quote_level == 2)
			expmap[current_pos] = '2';
		else
			expmap[current_pos] = '0';
		current_pos++;
		if (current_pos >= current_token_size - 1)
		{
			new_size = current_token_size * 2;
			new_token = ft_calloc(new_size, sizeof(char));
			new_expmap = ft_calloc(new_size, sizeof(char));
			if (!new_token || !new_expmap)
			{
				free(current_token);
				free(expmap);
				if (new_token)
					free(new_token);
				if (new_expmap)
					free(new_expmap);
				ft_error_exit("calloc");
			}
			ft_memcpy(new_token, current_token, current_token_size);
			ft_memcpy(new_expmap, expmap, current_token_size);
			free(current_token);
			free(expmap);
			current_token = new_token;
			expmap = new_expmap;
			current_token_size = new_size;
		}
		ptr++;
	}
	if (current_pos > 0)
	{
		current_token[current_pos] = '\0';
		expmap[current_pos] = '\0';
		token = create_token(current_token, expmap);
		update_tok_type(token, WORD);
		add_token_to_list(tok_lst, token);
	}
	free(current_token);
	free(expmap);
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
