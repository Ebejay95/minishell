/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_to_token2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:50:42 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 15:21:21 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	lex_redirection(t_token_data *data, t_list **tok_lst, char **ptr)
{
	t_token	*token;

	lex_space(data, tok_lst);
	if (*(*ptr + 1) == **ptr)
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
}

void	lex_regular_char(t_token_data *data, char c)
{
	data->current_token[data->current_pos] = c;
	if (data->quote_level == 1)
		data->expmap[data->current_pos] = '1';
	else if (data->quote_level == 2)
		data->expmap[data->current_pos] = '2';
	else
		data->expmap[data->current_pos] = '0';
	data->current_pos++;
}

void	resize_token_buffers(t_token_data *data)
{
	int		new_size;
	char	*new_token;
	char	*new_expmap;

	new_size = data->current_token_size * 2;
	new_token = ft_calloc(new_size, sizeof(char));
	new_expmap = ft_calloc(new_size, sizeof(char));
	if (!new_token || !new_expmap)
	{
		free(data->current_token);
		free(data->expmap);
		if (new_token)
			free(new_token);
		if (new_expmap)
			free(new_expmap);
		ft_error_exit("calloc");
	}
	ft_memcpy(new_token, data->current_token, data->current_token_size);
	ft_memcpy(new_expmap, data->expmap, data->current_token_size);
	free(data->current_token);
	free(data->expmap);
	data->current_token = new_token;
	data->expmap = new_expmap;
	data->current_token_size = new_size;
}

void	process_character(t_token_data *data, char **ptr, t_list **tok_lst)
{
	if (data->escape_next)
		lex_escape(data, **ptr);
	else if (**ptr == '\'' || **ptr == '"')
		lex_quote(data, **ptr);
	else if (ft_isspace(**ptr) && data->quote_level == 0)
		lex_space(data, tok_lst);
	else if (**ptr == '|' && data->quote_level == 0)
		lex_pipe(data, tok_lst);
	else if ((**ptr == '>' || **ptr == '<') && data->quote_level == 0)
		lex_redirection(data, tok_lst, ptr);
	else
		lex_regular_char(data, **ptr);
	if (data->current_pos >= data->current_token_size - 1)
		resize_token_buffers(data);
	(*ptr)++;
}

void	prompt_to_token(char *prompt, t_list **tok_lst)
{
	t_token_data	data;
	char			*ptr;

	ptr = prompt;
	initialize_token_data(&data);
	while (*ptr)
	{
		process_character(&data, &ptr, tok_lst);
		if (data.current_token == NULL || data.expmap == NULL)
		{
			ft_error_exit("Memory allocation failed in prompt_to_token");
		}
	}
	if (data.current_pos > 0)
		lex_space(&data, tok_lst);
	free(data.current_token);
	free(data.expmap);
}
