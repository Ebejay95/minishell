/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 15:35:24 by jonathanebe      ###   ########.fr       */
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

void	init_token_processing(char **curtok, char **expmap, int *curtoksz)
{
	*curtoksz = 10;
	*curtok = ft_calloc(*curtoksz, sizeof(char));
	*expmap = ft_calloc(*curtoksz, sizeof(char));
	if (!*curtok || !*expmap)
	{
		free(*curtok);
		free(*expmap);
		ft_error_exit("malloc");
	}
}

void	process_escape_char(char **ptr, char *current_token, char *expmap, int *current_pos, int quote_level)
{
	current_token[*current_pos] = **ptr;
	if (quote_level == 2)
		expmap[*current_pos] = '2';
	else
		expmap[*current_pos] = '0';
	(*current_pos)++;
	(*ptr)++;
}

void	process_quote(char **ptr, char *current_token, char *expmap, int *current_pos, int *quote_level)
{
	if (**ptr == '\'' && *quote_level == 0)
	{
		*quote_level = 1;
		current_token[*current_pos] = **ptr;
		expmap[*current_pos] = 'S';
	}
	else if (**ptr == '\'' && *quote_level == 1)
	{
		*quote_level = 0;
		current_token[*current_pos] = **ptr;
		expmap[*current_pos] = 'S';
	}
	else if (**ptr == '"' && *quote_level != 1)
	{
		current_token[*current_pos] = **ptr;
		expmap[*current_pos] = '0';
		if (*quote_level == 2)
			*quote_level = 0;
		else
			*quote_level = 2;
	}
	(*current_pos)++;
	(*ptr)++;
}

void	process_space(char **ptr, char *current_token, char *expmap, int *current_pos, t_list **tok_lst)
{
	t_token	*token;

	if (*current_pos > 0)
	{
		current_token[*current_pos] = '\0';
		expmap[*current_pos] = '\0';
		token = create_token(current_token, expmap);
		update_tok_type(token, WORD);
		add_token_to_list(tok_lst, token);
		*current_pos = 0;
	}
	(*ptr)++;
}

void	process_pipe(char **ptr, char *current_token, char *expmap, int *current_pos, t_list **tok_lst)
{
	t_token	*token;

	if (*current_pos > 0)
	{
		current_token[*current_pos] = '\0';
		expmap[*current_pos] = '\0';
		token = create_token(current_token, expmap);
		update_tok_type(token, WORD);
		add_token_to_list(tok_lst, token);
		*current_pos = 0;
	}
	token = create_token("|", "0");
	update_tok_type(token, PIPE);
	add_token_to_list(tok_lst, token);
	(*ptr)++;
}

void	process_redirection(char **ptr, char *current_token, char *expmap, int *current_pos, t_list **tok_lst)
{
	t_token	*token;

	if (*current_pos > 0)
	{
		current_token[*current_pos] = '\0';
		expmap[*current_pos] = '\0';
		token = create_token(current_token, expmap);
		update_tok_type(token, WORD);
		add_token_to_list(tok_lst, token);
		*current_pos = 0;
	}
	if (*((*ptr) + 1) == **ptr)
	{
		if (**ptr == '>')
			token = create_token(">>", "00");
		else
			token = create_token("<<", "00");
		(*ptr)++;
	}
	else
	{
		if (**ptr == '>')
			token = create_token(">", "00");
		else
			token = create_token("<", "00");
	}
	update_tok_type(token, REDIRECTION);
	add_token_to_list(tok_lst, token);
	(*ptr)++;
}

void	process_regular_char(char **ptr, char *current_token, char *expmap, int *current_pos, int quote_level)
{
	current_token[*current_pos] = **ptr;
	if (quote_level == 1)
		expmap[*current_pos] = '1';
	else if (quote_level == 2)
		expmap[*current_pos] = '2';
	else
		expmap[*current_pos] = '0';
	(*current_pos)++;
	(*ptr)++;
}

void	resize_token_buffers(char **current_token, char **expmap, int *current_token_size)
{
	int		new_size;
	char	*new_token;
	char	*new_expmap;

	new_size = *current_token_size * 2;
	new_token = ft_calloc(new_size, sizeof(char));
	new_expmap = ft_calloc(new_size, sizeof(char));
	if (!new_token || !new_expmap)
	{
		free(*current_token);
		free(*expmap);
		if (new_token)
			free(new_token);
		if (new_expmap)
			free(new_expmap);
		ft_error_exit("calloc");
	}
	ft_memcpy(new_token, *current_token, *current_token_size);
	ft_memcpy(new_expmap, *expmap, *current_token_size);
	free(*current_token);
	free(*expmap);
	*current_token = new_token;
	*expmap = new_expmap;
	*current_token_size = new_size;
}

void	prompt_to_token(char *prompt, t_list **tok_lst)
{
	int		current_pos;
	int		quote_level;
	int		escape_next;
	int		current_token_size;
	char	*current_token;
	char	*expmap;
	char	*ptr;
	t_token	*token;

	ptr = prompt;
	current_pos = 0;
	quote_level = 0;
	escape_next = 0;
	init_token_processing(&current_token, &expmap, &current_token_size);
	while (*ptr)
	{
		if (escape_next)
		{
			process_escape_char(&ptr, current_token, expmap, &current_pos, quote_level);
			escape_next = 0;
			continue ;
		}
		if (*ptr == '\'' || *ptr == '"')
		{
			process_quote(&ptr, current_token, expmap, &current_pos, &quote_level);
			continue ;
		}
		if (ft_isspace(*ptr) && quote_level == 0)
		{
			process_space(&ptr, current_token, expmap, &current_pos, tok_lst);
			continue ;
		}
		if (*ptr == '|' && quote_level == 0)
		{
			process_pipe(&ptr, current_token, expmap, &current_pos, tok_lst);
			continue ;
		}
		if ((*ptr == '>' || *ptr == '<') && quote_level == 0)
		{
			process_redirection(&ptr, current_token, expmap, &current_pos, tok_lst);
			continue ;
		}
		process_regular_char(&ptr, current_token, expmap, &current_pos, quote_level);
		if (current_pos >= current_token_size - 1)
		{
			resize_token_buffers(&current_token, &expmap, &current_token_size);
		}
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

void	expand_toklst(t_minishell *m, t_list **tok_lst)
{

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
