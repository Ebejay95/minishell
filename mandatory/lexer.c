/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/19 01:27:04 by jeberle          ###   ########.fr       */
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

void	prompt_to_token(t_minishell *m)
{
	char	*start;
	char	*work;
	char	*token_str;
	int		start_pos;
	int		end_pos;
	t_token	*token;
	char	*end;
	char	*combined_token;
	char	*final_token;

	start = m->prompt;
	work = m->prompt;
	while (*work)
	{
		if (*work == '\'')
		{
			if (work > start)
			{
				while (work > start && !isspace(*(work - 1)) && *(work - 1) != '\'')
					work--;
				token_str = strndup(start, work - start);
				start_pos = start - m->prompt;
				end_pos = work - m->prompt - 1;
				token = create_token(token_str, start_pos, end_pos, 1);
				add_token_to_list(&m->tok_lst, token);
				free(token_str);
				start = work;
			}
			start++;
			while (*work && *work != '\'')
				work++;
			if (*work == '\'')
			{
				start--;
				end = work + 1;
				while (*end && !isspace(*end))
					end++;
				token_str = strndup(start + 1, work - start - 1);
				combined_token = malloc(work - start + (end - work) + 1);
				strncpy(combined_token, start, work - start + 1);
				strncpy(combined_token + (work - start + 1), work + 1, end - work - 1);
				combined_token[(work - start + 1) + (end - work - 1)] = '\0';
				final_token = remove_chars(combined_token, "'");
				start_pos = start - m->prompt;
				end_pos = end - m->prompt - 1;
				token = create_token(final_token, start_pos, end_pos, 0);
				add_token_to_list(&m->tok_lst, token);
				free(final_token);
				free(combined_token);
				work = end;
				start = work;
			}
			else
			{
				break ;
			}
		}
		else if (isspace(*work))
		{
			if (work > start)
			{
				token_str = strndup(start, work - start);
				start_pos = start - m->prompt;
				end_pos = work - m->prompt - 1;
				token = create_token(token_str, start_pos, end_pos, 1);
				add_token_to_list(&m->tok_lst, token);
				free(token_str);
			}
			work++;
			start = work;
		}
		else
			work++;
	}
	if (*start)
	{
		token_str = strndup(start, work - start);
		start_pos = start - m->prompt;
		end_pos = work - m->prompt - 1;
		token = create_token(token_str, start_pos, end_pos, 1);
		add_token_to_list(&m->tok_lst, token);
		free(token_str);
	}
}

void	splice_token_by(t_list *node, t_token *token, t_toktype lookfor, char *skippers)
{
	t_list		*new_element;
	t_token		*newtok;
	t_list		*current;
	t_segment	**news;
	int			index;

	if (!ft_strstr(skippers, toktype_to_str(token->token)))
	{
		current = node;
		news = lex(token, lookfor);
		index = 0;
		while (news[index])
		{
			if (lookfor == VARIABLE && strlen(news[index]->str) == 0)
			{
				index++;
				continue ;
			}
			if (index != 0)
			{
				newtok = create_token(news[index]->str, 0, ft_strlen(news[index]->str), 1);
				update_tok_type(newtok, news[index]->type);
				new_element = ft_lstnew((void *)newtok);
				if (!new_element)
					return ;
				new_element->next = current->next;
				current->next = new_element;
				current = current->next;
			}
			else
			{
				token->str = news[index]->str;
				update_tok_type(token, news[index]->type);
			}
			index++;
		}
	}
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
			cur_content->str = remove_chars(cur_content->str, "\"");
			update_tok_type(cur_content, WORD);
		}
		current = current->next;
	}
}

			// trenne ' =sdf' als einzelnes = auf und interpretiere sdf als argument
			// bei 'sdfdsf=' lesgt eine variabkendeklaration vor bei 'sdfsdf= ' auch aber der wert dahinter wird nicht mitgenommen

// void set_rdrct_token_details(t_token *token, int fdin, int fdout, const char *rdrctype) {
//     if (token == NULL) return;
//     token->token = REDIRECTION;
//     token->detail.redirection.fdin = fdin;
//     token->detail.redirection.fdout = fdout;
//     token->detail.redirection.rdrctype = rdrctype;
// }

// void set_command_arg_token_details(t_token *token, int arglen, int is_command) {
//     if (token == NULL) return;
//     token->token = is_command ? COMMAND : ARGUMENT;
//     token->detail.arglen = arglen;
// }

// void set_pipe_token_details(t_token *token, int fdin, int fdout) {
//     if (token == NULL) return;
//     token->token = PIPE;
//     token->detail.pipe.fdin = fdin;
//     token->detail.pipe.fdout = fdout;
// }

void	analyze_tokens(t_minishell *m)
{
	t_list		*current;
	t_token		*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (cur_content->expand)
		{
			splice_token_by(current, cur_content, REDIRECTION, "");
			splice_token_by(current, cur_content, PIPE, "");
		}
		current = current->next;
	}
	expand_toklst(m);
}

//splice_token_by(current, cur_content, GETEXSTATE, "Variable");
//splice_token_by(current, cur_content, VARIABLE, "Ges");
//current = m->tok_lst;
//while (current != NULL)
//{
//	cur_content = (t_token *)current->content;
//	if (cur_content->expand)
//		splice_token_by(current, cur_content, WORD, "");
//	current = current->next;
//}

void	lex_prompt(t_minishell *m)
{
	char	*tmpp;

	tmpp = remove_chars(m->prompt, "\n");
	m->prompt = tmpp;
	prompt_to_token(m);
	ft_printf(Y"TOKENLIST:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
	analyze_tokens(m);
	ft_printf(Y"TOKENLIST:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
}
