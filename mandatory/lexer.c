/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/16 22:21:01 by jeberle          ###   ########.fr       */
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

void	prompt_to_token(t_minishell *m)
{
	char	*start;
	char	*work;
	char	*token_str;
	int		start_pos;
	int		end_pos;
	t_token	*token;

	start = m->prompt;
	work = m->prompt;
	while (*work)
	{
		if (*work == '\'')
		{
			if (work > start)
			{
				token_str = remove_chars(strndup(start, work - start), "\"\n");
				start_pos = start - m->prompt;
				end_pos = work - m->prompt - 1;
				token = create_token(token_str, start_pos, end_pos, 1);
				add_token_to_list(&m->tok_lst, token);
				free(token_str);
			}
			start = ++work;
			while (*work && *work != '\'')
				work++;
			if (*work == '\'')
			{
				token_str = strndup(start, work - start);
				start_pos = start - m->prompt;
				end_pos = work - m->prompt;
				token = create_token(token_str, start_pos, end_pos, 0);
				add_token_to_list(&m->tok_lst, token);
				free(token_str);
				work++;
				start = work;
			}
			else
				break ;
		}
		else
			work++;
	}
	if (*start)
	{
		token_str = remove_chars(strndup(start, work - start), "\"\n");
		start_pos = start - m->prompt;
		end_pos = work - m->prompt - 1;
		token = create_token(token_str, start_pos, end_pos, 1);
		add_token_to_list(&m->tok_lst, token);
		free(token_str);
	}
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
void	splice_token_by(t_list *node, t_token *token, t_toktype lookfor)
{
	t_list		*new_element;
	t_token		*newtok;
	t_list		*current;
	t_segment	**news;
	int			index;

	current = node;
	news = lex(token->str, lookfor);
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
			splice_token_by(current, cur_content, REDIRECTION);
			splice_token_by(current, cur_content, PIPE);
			splice_token_by(current, cur_content, GETEXSTATE);
			splice_token_by(current, cur_content, VARIABLE);
			// trenne ' =sdf' als einzelnes = auf und interpretiere sdf als argument
			// bei 'sdfdsf=' lesgt eine variabkendeklaration vor bei 'sdfsdf= ' auch aber der wert dahinter wird nicht mitgenommen
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
	ft_printf(Y"TOKENLIST BEFORE ANALYSING:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
	analyze_tokens(m);
	ft_printf(Y"TOKENLIST GANZ AM ENDE:\n"D);
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
