/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_neu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 11:42:20 by chorst            #+#    #+#             */
/*   Updated: 2024/08/09 13:11:01 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static void	afterbreakup(t_minishell *m)
{
	t_list	*current;
	t_list	*new_node;
	t_token	*cur;
	t_token	*new_token;
	int		wordcount;
	int		i;
	char	*work;
	char	**words;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur = (t_token *)current->content;
		if (!ft_strstr(cur->expmap, "X") && ft_strstr(cur->expmap, "E"))
		{
			work = whitespace_handler(cur->str);
			wordcount = ft_count_words(work, ' ');
			words = ft_split(work, ' ');
			free(cur->str);
			cur->str = words[0];
			i = 1;
			while (i < wordcount)
			{
				new_token = create_token(words[i], cur->expmap);
				update_tok_type(new_token, WORD);
				new_node = ft_lstnew(new_token);
				new_node->next = current->next;
				current->next = new_node;
				current = new_node;
				i++;
			}
			free(words);
		}
		current = current->next;
	}
}

static void	prompt_to_token(t_minishell *m)
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

	current_pos = 0;
	quote_level = 0;
	escape_next = 0;
	current_token_size = 10;
	ptr = m->prompt;
	current_token = malloc(current_token_size);
	expmap = malloc(current_token_size);
	if (!current_token || !expmap)
		ft_error_exit("malloc");
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
		if (*ptr == '\'')
		{
			quote_level = 1 - quote_level;
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
		if ((ft_isspace(*ptr) || *ptr == '|') && quote_level == 0)
		{
			if (current_pos > 0)
			{
				current_token[current_pos] = '\0';
				expmap[current_pos] = '\0';
				token = create_token(current_token, expmap);
				update_tok_type(token, WORD);
				add_token_to_list(&m->tok_lst, token);
				current_pos = 0;
			}
			if (*ptr == '|')
			{
				token = create_token("|", "0");
				update_tok_type(token, PIPE);
				add_token_to_list(&m->tok_lst, token);
			}
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
				add_token_to_list(&m->tok_lst, token);
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
			add_token_to_list(&m->tok_lst, token);
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
		if (current_pos >= current_token_size)
		{
			current_token_size *= 2;
			new_token = realloc(current_token, current_token_size);
			new_expmap = realloc(expmap, current_token_size);
			if (!new_token || !new_expmap)
			{
				free(current_token);
				free(expmap);
				ft_error_exit("realloc");
			}
			current_token = new_token;
			expmap = new_expmap;
		}
		ptr++;
	}
	if (current_pos > 0)
	{
		current_token[current_pos] = '\0';
		expmap[current_pos] = '\0';
		token = create_token(current_token, expmap);
		update_tok_type(token, WORD);
		add_token_to_list(&m->tok_lst, token);
	}
	free(current_token);
	free(expmap);
}

//free(cur_content->expmap);
//cur_content->expmap = NULL;
//ft_printf(R"%s\n"D, cur_content->str);
//ft_printf(G"%s\n"D, cur_content->expmap);
static void	expand_toklst(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		expand_token(m, cur_content);
		current = current->next;
	}
}

static void	detect_unclosed_quotes(t_minishell *minishell)
{
	char	*current_char;
	char	open_quote;
	int		inside_quote;

	current_char = minishell->prompt;
	open_quote = 0;
	inside_quote = 0;
	while (*current_char != '\0')
	{
		if (!inside_quote && (*current_char == '\'' || *current_char == '\"'))
		{
			open_quote = *current_char;
			inside_quote = 1;
		}
		else if (inside_quote && *current_char == open_quote)
			inside_quote = 0;
		current_char++;
	}
	if (inside_quote)
		pic_err(minishell, 2, "unclosed quotes");
}

// static void	simplify_quotes(char *str)
// {
// 	char		*result;
// 	char		*write_pos;
// 	const char	*read_pos;
// 	int			length;

// 	read_pos = str;
// 	length = ft_strlen(str);
// 	result = malloc(length + 1);
// 	if (!result)
// 		return (NULL);
// 	write_pos = result;
// 	while (*read_pos != '\0')
// 	{
// 		if ((read_pos[0] == '\'' || read_pos[0] == '\"')
// 			&& read_pos[1] == read_pos[0])
// 		{
// 			read_pos += 2;
// 			continue ;
// 		}
// 		*write_pos++ = *read_pos++;
// 	}
// 	*write_pos = '\0';
// 	return (result);
// }


void	add_token_to_list(t_list **lst, t_token *token)
{
	t_list	*new_element;
	t_list	*current;

	if (token)
	{
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
}

//void	lex_prompt(t_minishell *minishell)
//{
//	char	*temp;
//
//	temp = remove_chars(minishell->prompt, "\n");
//	minishell->prompt = temp;
//	detect_unclosed_quotes(minishell);
//	prompt_to_token(minishell);
//	expand_toklst(minishell);
//	afterbreakup(minishell);
//	//if (DEBUG == 1)
//	//{
//	//	ft_printf(Y"TOKENLIST:\n"D);
//	//	ft_lstput(&(minishell->tok_lst), put_token, '\n');
//	//}
//}
