/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_to_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:50:42 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 15:21:50 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	initialize_token_data(t_token_data *data)
{
	data->current_pos = 0;
	data->quote_level = 0;
	data->escape_next = 0;
	data->current_token_size = 10;
	data->current_token = ft_calloc(data->current_token_size, sizeof(char));
	data->expmap = ft_calloc(data->current_token_size, sizeof(char));
	if (!data->current_token || !data->expmap)
	{
		free(data->current_token);
		free(data->expmap);
		ft_error_exit("malloc");
	}
}

void	lex_escape(t_token_data *data, char c)
{
	data->current_token[data->current_pos] = c;
	if (data->quote_level == 2)
		data->expmap[data->current_pos] = '2';
	else
		data->expmap[data->current_pos] = '0';
	data->current_pos++;
	data->escape_next = 0;
}

void	lex_quote(t_token_data *data, char c)
{
	if (c == '\'' && data->quote_level == 0)
	{
		data->quote_level = 1;
		data->current_token[data->current_pos] = c;
		data->expmap[data->current_pos] = 'S';
		data->current_pos++;
	}
	else if (c == '\'' && data->quote_level == 1)
	{
		data->quote_level = 0;
		data->current_token[data->current_pos] = c;
		data->expmap[data->current_pos] = 'S';
		data->current_pos++;
	}
	else if (c == '"' && data->quote_level != 1)
	{
		data->current_token[data->current_pos] = c;
		data->expmap[data->current_pos] = '0';
		data->current_pos++;
		if (data->quote_level == 2)
			data->quote_level = 0;
		else
			data->quote_level = 2;
	}
}

void	lex_space(t_token_data *data, t_list **tok_lst)
{
	t_token	*token;

	if (data->current_pos > 0)
	{
		data->current_token[data->current_pos] = '\0';
		data->expmap[data->current_pos] = '\0';
		token = create_token(data->current_token, data->expmap);
		update_tok_type(token, WORD);
		add_token_to_list(tok_lst, token);
		data->current_pos = 0;
	}
}

void	lex_pipe(t_token_data *data, t_list **tok_lst)
{
	t_token	*token;

	lex_space(data, tok_lst);
	token = create_token("|", "0");
	update_tok_type(token, PIPE);
	add_token_to_list(tok_lst, token);
}
