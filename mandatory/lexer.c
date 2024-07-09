/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/09 10:39:55 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	add_token_to_list(t_list **lst, t_token *token)
{
	t_list	*new_element;
	t_list	*current;

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

// Ich habe eine Funktion geschrieben die remove_chars heisst.
// Du kannst annstatt remove_backslashes, diese Funktion verwenden,
// um die Backslashes aus deinem String zu entfernen.
// Sie ist flexibler und kann auch andere Zeichen entfernen, falls gewünscht.
// Sie befindet sich in der Datei mandatory/remove_chars.c
char	*remove_backslashes(char *str)
{
	int		count;
	char	*temp;
	char	*new_str;
	char	*new_str_ptr;

	count = 0;
	temp = str;
	while (*temp)
	{
		if (*temp != '\\')
			count++;
		temp++;
	}
	new_str = (char *) malloc((count + 1) * sizeof(char));
	if (new_str == NULL)
		exit(EXIT_FAILURE);
	new_str_ptr = new_str;
	while (*str)
	{
		if (*str != '\\')
		{
			*new_str_ptr = *str;
			new_str_ptr++;
		}
		str++;
	}
	*new_str_ptr = '\0';
	return (new_str);
}

void prompt_to_token(t_minishell *m)
{
	t_token	*token;
	int		is_single;
	int		is_double;
	int		start_pos;
	int		current_pos;
	char	*token_str;
	char	*work;
	char	quote_type;

	work = m->prompt;
	is_single = 0;
	is_double = 0;
	start_pos = 0;
	current_pos = 0;
	quote_type = 0;
	while (work[current_pos])
	{
		if (work[current_pos] == '\\' && (work[current_pos + 1] == '\'' || work[current_pos + 1] == '"' || work[current_pos + 1] == '\\'))
			current_pos++;
		else if (work[current_pos] == '\'' && !is_double)
		{
			if (!is_single)
			{
				if (current_pos - start_pos > 0)
				{
					token_str = ft_strndup(work + start_pos, current_pos - start_pos);
					if (token_str)
					{
						token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos, 1);
						add_token_to_list(&m->tok_lst, token);
					}
				}
				start_pos = current_pos;
				is_single = 1;
				quote_type = '\'';
			}
			else
			{
				is_single = 0;
				quote_type = 0;
				token_str = ft_strndup(work + start_pos, current_pos - start_pos + 1);
				if (token_str)
				{
					token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos + 1, 0);
					add_token_to_list(&m->tok_lst, token);
				}
				start_pos = current_pos + 1;
			}
		}
		else if (work[current_pos] == '"' && !is_single)
		{
			if (!is_double)
			{
				if (current_pos - start_pos > 0)
				{
					token_str = ft_strndup(work + start_pos, current_pos - start_pos);
					if (token_str)
					{
						token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos, 1);
						add_token_to_list(&m->tok_lst, token);
					}
				}
				start_pos = current_pos;
				is_double = 1;
				quote_type = '"';
			}
			else
			{
				is_double = 0;
				quote_type = 0;
				token_str = ft_strndup(work + start_pos, current_pos - start_pos + 1);
				if (token_str)
				{
					token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos + 1, 0);
					add_token_to_list(&m->tok_lst, token);
				}
				start_pos = current_pos + 1;
			}
		}
		else if (ft_isspace(work[current_pos]) && !is_single && !is_double)
		{
			if (current_pos - start_pos > 0)
			{
				token_str = ft_strndup(work + start_pos, current_pos - start_pos);
				if (token_str)
				{
					token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos, 1);
					add_token_to_list(&m->tok_lst, token);
				}
				quote_type = 0;
			}
			start_pos = current_pos + 1;
		}
		current_pos++;
	}
	if (current_pos - start_pos > 0)
	{
		token_str = ft_strndup(work + start_pos, current_pos - start_pos);
		if (token_str)
		{
			token = create_token(remove_backslashes(token_str), start_pos, current_pos - start_pos, 1);
			add_token_to_list(&m->tok_lst, token);
		}
	}
	if (is_single || is_double)
		ft_printf(R "Error: Mismatched quotes in input.\n"D);
}

int	contains_quotes(char *str)
{
	if (ft_strcontains(str, '"') || ft_strcontains(str, '\''))
		return (1);
	return (0);
}

/// ermittle nicht die dinger einzeln sondern pruefe zeichen fuer zeiechen,... find alle faelle...
// >
// $WHITESPACE, EXPANDBELE, ?,

// Token-Typ                Beschreibung                       Regex                   Beispiel 1                      Beispiel 2
// --------------------------------------------------------------------------------------------------------
// REDIRECTION              Einzelne Umleitung                 ^>$                     > output.txt                   2> error.log
// APPEND_REDIRECTION       Anfügende Umleitung                ^>>$                    >> output.txt                  2>> error.log
// HEREDOC                  Here-Document                      ^<<$                    << EOF                         << MYTAG
// FD_REDIRECTION           Datei-Deskriptor Umleitung         ^[0-9]+>$               1> output.txt                  2> error.log
// PIPE                     Pipeline Operator                  ^|$                     ls | grep txt                  ps aux | less
// COMMAND                  Befehl                             ^[a-zA-Z0-9_\-\/]+$     ls                            cat
// ARGUMENT                 Argument                           ^.+$                    -l                            --help
// VARIABLE_ASSIGNMENT      Variablenzuweisung                 ^[a-zA-Z_][a-zA-Z0-9_]*=.*$  VAR=value                     PATH=/usr/bin
// SUBSHELL                 Subshell                           ^\(.*\)$                (cd /tmp; ls)                  (echo "Hello")
// VARIABLE                 Variable                           ^\$[a-zA-Z_][a-zA-Z0-9_]*$  $PATH                        $HOME
// SPECIAL_VARIABLE         Spezielle Variable                 ^$(?                    $?                             $#
// EXPANSION                Parameter Expansion                ^\$\{.*\}$              ${VAR}                         ${PATH}
// UNSET                    Nicht festgelegter Typ             N/A                     N/A                            N/A
void	analyze_redirect_tokens(t_list *node, t_token *token)
{
	t_list		*new_element;
	t_token		*newtok;
	t_list		*current;
	t_segment	**news;
	int			index;

	current = node;
	if (!contains_quotes(token->str))
	{
		news = lex_redirections(token->str);
		index = 0;
		while (news[index])
		{
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

void	analyze_pipe_tokens(t_list *node, t_token *token)
{
	t_list		*new_element;
	t_token		*newtok;
	t_list		*current;
	t_segment	**news;
	int			index;

	current = node;
	if (!contains_quotes(token->str))
	{
		news = lex_pipes(token->str);
		index = 0;
		while (news[index])
		{
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

void	analyze_tokens(t_minishell *m)
{
	t_list		*current;

	current = m->tok_lst;
	while (current != NULL)
	{
		analyze_redirect_tokens(current, (t_token *)current->content);
		analyze_pipe_tokens(current, (t_token *)current->content);
		analyze_pipe_tokens(current, (t_token *)current->content);
		// trenne ' =sdf' als einzelnes = auf und interpretiere sdf als argument
		// bei 'sdfdsf=' lesgt eine variabkendeklaration vor bei 'sdfsdf= ' auch aber der wert dahinter wird nicht mitgenommen
		current = current->next;
	}
}

void	lex_prompt(t_minishell *m)
{
	prompt_to_token(m);
	ft_printf(Y"TOKENLIST BEFORE ANALYSING:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
	analyze_tokens(m);
	ft_printf(Y"TOKENLIST:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
}

// t_segment	*find_seg_by_start(t_segment **segs, int search)
// {
// 	int	segdex;

// 	segdex = 0;
// 	while (segs[segdex])
// 	{
// 		if (segs[segdex]->start == search)
// 			return (segs[segdex]);
// 		segdex++;
// 	}
// 	return (NULL);
// }

// t_segment	*find_seg_by_end(t_segment **segs, int search)
// {
// 	int	segdex;

// 	segdex = 0;
// 	while (segs[segdex])
// 	{
// 		if (segs[segdex]->end == search)
// 			return (segs[segdex]);
// 		segdex++;
// 	}
// 	return (NULL);
// }

// t_segment	*find_seg_by_str(t_segment **segs, char *search)
// {
// 	int	segdex;

// 	segdex = 0;
// 	while (segs[segdex])
// 	{
// 		if (ft_strcmp(segs[segdex]->str, search) == 0)
// 			return (segs[segdex]);
// 		segdex++;
// 	}
// 	return (NULL);
// }
