/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/21 19:03:23 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

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

void	expand_token(t_token *token)
{
	char	*new_str;
	char	*var_start;
	char	*var_end;
	char	*env_value;
	size_t	new_str_len;
	char	*ptr;
	size_t	pre_var_len;
	char	temp;
	// EXITCODE $? ggf hier auch expanden... 
// jeberle@2-E-4 minishell % echo$?
// zsh: command not found: echo0
	new_str = NULL;
	var_start = NULL;
	var_end = NULL;
	env_value = NULL;
	new_str_len = 0;
	ptr = token->str;
	pre_var_len = 0;
	new_str = malloc(1);
	if (new_str == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	new_str[0] = '\0';
	while (*ptr != '\0')
	{
		if (*ptr == '$' && *(ptr + 1) != '?' && (*(ptr + 1) == '_' || ft_isalnum(*(ptr + 1))))
		{
			var_start = ptr + 1;
			var_end = var_start;
			while (*var_end != '\0' && (ft_isalnum(*var_end) || *var_end == '_'))
			{
				var_end++;
			}
			temp = *var_end;
			*var_end = '\0';
			env_value = getenv(var_start);
			*var_end = temp;
			if (env_value != NULL)
			{
				new_str_len += strlen(env_value);
				new_str = realloc(new_str, new_str_len + 1);
				if (new_str == NULL)
				{
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}
				strcat(new_str, env_value);
			}
			pre_var_len = var_start - ptr - 1;
			new_str_len += pre_var_len;
			new_str = realloc(new_str, new_str_len + 1);
			if (new_str == NULL)
			{
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			strncat(new_str, ptr, pre_var_len);
			ptr = var_end;
		}
		else
		{
			new_str_len++;
			new_str = realloc(new_str, new_str_len + 1);
			if (new_str == NULL)
			{
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			strncat(new_str, ptr, 1);
			ptr++;
		}
	}
	free(token->str);
	token->str = new_str;
}

int	is_redirection_pattern(char *str)
{
	int	i;

	i = 0;
	if ((str[i] == '>' || str[i] == '<'))
	{
		while (str[i] == '>' || str[i] == '<')
			i++;
		if (str[i] == '\0')
		{
			ft_fprintf(2, "bash: syntax error near unexpected token `newline'\n");
			return (-1);
		}
		if (str[i] == '|')
		{
			ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
			return (-1);
		}
		return (i);
	}
	return (0);
}

int	is_pipe_pattern(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '|')
	{
		i++;
		if (str[i] == '|')
		{
			ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
			return (-1);
		}
		return (i);
	}
	return (0);
}

void	process_word_token(t_minishell *m, char **start, char **work)
{
	char	*token_str;
	int		start_pos;
	int		end_pos;
	t_token	*token;

	if (*work > *start)
	{
		token_str = remove_chars(strndup(*start, *work - *start), "'");
		start_pos = *start - m->prompt;
		end_pos = *work - m->prompt - 1;
		token = create_token(token_str, start_pos, end_pos, 0);
		add_token_to_list(&m->tok_lst, token);
		free(token_str);
	}
	*start = *work;
}

void	prompt_to_token(t_minishell *m)
{
	char	*start;
	char	*work;
	int		metachars_len;
	char	*token_str;
	int		start_pos;
	int		end_pos;
	t_token	*token;

	//hIer expands array erstellen... fuer exoander
	while (*work) {
		if (*work == '\'') {
			work++;
			while (*work && *work != '\'')
			{
				work++;
			}
			if (*work == '\'')
			{
				work++;
			}
		}
		else if (*work == '>' || *work == '<')
		{
			process_word_token(m, &start, &work);
			metachars_len = is_redirection_pattern(work);
			if (metachars_len > 0)
			{
				token_str = ft_strndup(work, metachars_len);
				start_pos = work - m->prompt;
				end_pos = start_pos + metachars_len - 1;
				token = create_token(token_str, start_pos, end_pos, 0);
				update_tok_type(token, REDIRECTION);
				add_token_to_list(&m->tok_lst, token);
				free(token_str);
				work += metachars_len;
				start = work;
			}
		}
		else if (*work == '|')
		{
			process_word_token(m, &start, &work);
			metachars_len = is_pipe_pattern(work);
			if (metachars_len > 0) {
				token_str = ft_strndup(work, metachars_len);
				start_pos = work - m->prompt;
				end_pos = start_pos + metachars_len - 1;
				token = create_token(token_str, start_pos, end_pos, 0);
				update_tok_type(token, PIPE);
				add_token_to_list(&m->tok_lst, token);
				free(token_str);
				work += metachars_len;
				start = work;
			}
		}
		else if (isspace(*work))
		{
			process_word_token(m, &start, &work);
			work++;
			start = work;
		}
		else
		{
			work++;
		}
	}
	process_word_token(m, &start, &work);
}

void	expand_toklst(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (cur_content->expand)
		{
			expand_token(cur_content);
		}
		current = current->next;
	}
}

void	lex_prompt(t_minishell *m)
{
	char	*tmpp;

	tmpp = remove_chars(m->prompt, "\n");
	m->prompt = tmpp;
	prompt_to_token(m);
	expand_toklst(m);
	ft_printf(Y"TOKENLIST:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
}
