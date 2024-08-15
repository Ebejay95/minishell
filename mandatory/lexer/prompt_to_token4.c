/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_to_token4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 20:48:22 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 20:48:30 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_escape_char(t_tokenizer_state *state)
{
	state->current_token[state->current_pos] = *state->ptr;
	if (state->quote_level == 2)
		state->expmap[state->current_pos] = '2';
	else
		state->expmap[state->current_pos] = '0';
	state->current_pos++;
	state->ptr++;
}

void	handle_backslash(t_tokenizer_state *state)
{
	state->escape_next = 1;
	state->current_token[state->current_pos] = *state->ptr;
	state->expmap[state->current_pos] = '0';
	state->current_pos++;
	state->ptr++;
}

void	handle_single_quote(t_tokenizer_state *state)
{
	if (state->quote_level == 0)
		state->quote_level = 1;
	else if (state->quote_level == 1)
		state->quote_level = 0;
	state->current_token[state->current_pos] = *state->ptr;
	state->expmap[state->current_pos] = 'S';
	state->current_pos++;
	state->ptr++;
}
