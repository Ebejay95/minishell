/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_to_token2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:50:42 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 20:47:48 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_double_redirection(t_tokenizer_state *state)
{
	if (*state->ptr == '>')
		create_rdrct_token(state, ">>", "00");
	else
		create_rdrct_token(state, "<<", "00");
	state->ptr++;
}

void	handle_single_redirection(t_tokenizer_state *state)
{
	if (*state->ptr == '>')
		create_rdrct_token(state, ">", "0");
	else
		create_rdrct_token(state, "<", "0");
}

void	handle_redirection(t_tokenizer_state *state)
{
	handle_current_token(state);
	if (*(state->ptr + 1) == *state->ptr)
		handle_double_redirection(state);
	else
		handle_single_redirection(state);
	state->ptr++;
}

void	handle_regular_char(t_tokenizer_state *state)
{
	state->current_token[state->current_pos] = *state->ptr;
	if (state->quote_level == 1)
		state->expmap[state->current_pos] = '1';
	else if (state->quote_level == 2)
		state->expmap[state->current_pos] = '2';
	else
		state->expmap[state->current_pos] = '0';
	state->current_pos++;
	state->ptr++;
}

void	resize_token_buffers(t_tokenizer_state *state)
{
	int		new_size;
	char	*new_token;
	char	*new_expmap;

	new_size = state->current_token_size * 2;
	new_token = ft_calloc(new_size, sizeof(char));
	new_expmap = ft_calloc(new_size, sizeof(char));
	if (!new_token || !new_expmap)
	{
		free(state->current_token);
		free(state->expmap);
		if (new_token)
			free(new_token);
		if (new_expmap)
			free(new_expmap);
		ft_error_exit("calloc");
	}
	ft_memcpy(new_token, state->current_token, state->current_token_size);
	ft_memcpy(new_expmap, state->expmap, state->current_token_size);
	free(state->current_token);
	free(state->expmap);
	state->current_token = new_token;
	state->expmap = new_expmap;
	state->current_token_size = new_size;
}
