/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_to_token3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 20:47:41 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/15 20:47:51 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	handle_double_quote(t_tokenizer_state *state)
{
	state->current_token[state->current_pos] = *state->ptr;
	state->expmap[state->current_pos] = '0';
	state->current_pos++;
	if (state->quote_level == 2)
		state->quote_level = 0;
	else
		state->quote_level = 2;
	state->ptr++;
}

void	handle_space(t_tokenizer_state *state)
{
	t_token	*token;

	if (state->current_pos > 0)
	{
		state->current_token[state->current_pos] = '\0';
		state->expmap[state->current_pos] = '\0';
		token = create_token(state->current_token, state->expmap);
		update_tok_type(token, WORD);
		add_token_to_list(state->tok_lst, token);
		state->current_pos = 0;
	}
	state->ptr++;
}

void	handle_pipe(t_tokenizer_state *state)
{
	t_token	*token;

	if (state->current_pos > 0)
	{
		state->current_token[state->current_pos] = '\0';
		state->expmap[state->current_pos] = '\0';
		token = create_token(state->current_token, state->expmap);
		update_tok_type(token, WORD);
		add_token_to_list(state->tok_lst, token);
		state->current_pos = 0;
	}
	token = create_token("|", "0");
	update_tok_type(token, PIPE);
	add_token_to_list(state->tok_lst, token);
	state->ptr++;
}

void	handle_current_token(t_tokenizer_state *state)
{
	t_token	*token;

	if (state->current_pos > 0)
	{
		state->current_token[state->current_pos] = '\0';
		state->expmap[state->current_pos] = '\0';
		token = create_token(state->current_token, state->expmap);
		update_tok_type(token, WORD);
		add_token_to_list(state->tok_lst, token);
		state->current_pos = 0;
	}
}

void	create_rdrct_token(t_tokenizer_state *state, char *rdrct, char *expmap)
{
	t_token	*token;

	token = create_token(rdrct, expmap);
	update_tok_type(token, REDIRECTION);
	add_token_to_list(state->tok_lst, token);
}
