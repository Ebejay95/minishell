/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/21 15:25:01 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that prints the lexer structure
void	lex_prompt(t_minishell *m)
{
	m->lexer.pipe_buffer = get_segs(m->prompt, "|");
	m->lexer.quote_buffer = get_quote_segs(m, '\'');
	m->lexer.dquote_buffer = get_quote_segs(m, '"');
	m->lexer.inrdrct_buffer = get_segs(m->prompt, "<");
	m->lexer.outrdrct_buffer = get_segs(m->prompt, ">");
	m->lexer.appoutrdrct_buffer = get_segs(m->prompt, ">>");
	m->lexer.heredoc_buffer = get_segs(m->prompt, "<<");
	m->lexer.space_buffer = get_segs(m->prompt, " ");
	m->lexer.option_buffer = get_segs(m->prompt, "-");
	m->lexer.ampersand_buffer = get_segs(m->prompt, "&");
	m->lexer.semicolon_buffer = get_segs(m->prompt, ";");
	m->lexer.variable_buffer = get_set_segs(m, VS);
	m->lexer.redirection_buffer = get_set_segs(m, RDRCSET);
	m->lexer.equal_buffer = get_segs(m->prompt, "=");
}

// Returns a cleaned prompt without leading or trailing delimiters
char	*input_cleaner(char *prompt)
{
	int		i;
	int		j;
	char	*temp_prompt;
	char	*clean_prompt;

	i = 0;
	j = 0;
	temp_prompt = ft_strtrim(prompt, " \t\n\v\f\r");
	clean_prompt = (char *)malloc(strlen(temp_prompt) + 1);
	while (temp_prompt[i])
	{
		if (temp_prompt[i] != 32 && !(temp_prompt[i] >= 9
				&& temp_prompt[i] <= 13))
			clean_prompt[j++] = temp_prompt[i];
		else if (i > 0 && !(clean_prompt[j - 1] == 32 || (clean_prompt[j - 1] >= 9 && clean_prompt[j - 1] <= 13)))
			clean_prompt[j++] = ' ';
		i++;
	}
	clean_prompt[j] = '\0';
	free(temp_prompt);
	return (clean_prompt);
}
