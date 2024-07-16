/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:30:06 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/16 15:54:07 by jeberle          ###   ########.fr       */
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


int after_quote(const char *str)
{
    int i = 1; // Start after the initial quote
    while (str[i] && str[i] != str[0])
    {
        if (str[i] == '\\' && str[i + 1])
            i += 2; // Skip escaped character
        else
            i++;
    }
    return (str[i] == str[0]) ? i + 1 : i;
}

int token_type(char c)
{
    if (c == '|') return 1;
    if (c == '&') return 2;
    if (c == ';') return 3;
    if (c == '(') return 4;
    if (c == ')') return 5;
    if (c == '<') return 6;
    if (c == '>') return 7;
    return 0;
}

void prompt_to_token(t_minishell *m) {
    char *start = NULL;
    char *end = NULL;
    char *work = m->prompt;

    while (*work) {
        if (*work == '\'') {
            start = ++work;
            end = start;
            while (*end && *end != '\'') {
                end++;
            }

            if (*end == '\'') {
                char *token_str = strndup(start, end - start); 
                char *cleaned_str = remove_chars(token_str, "\\");  
                int start_pos = start - m->prompt;
                int size = end - start;

                t_token *token = create_token(cleaned_str, start_pos, size, 0);
                add_token_to_list(&m->tok_lst, token);

                free(token_str);
                free(cleaned_str);

                work = end + 1;
            } else {
                break;
            }
        } else {
            work++;
        }
    }
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
		//analyze_var_tokens(current, (t_token *)current->content);
		// trenne ' =sdf' als einzelnes = auf und interpretiere sdf als argument
		// bei 'sdfdsf=' lesgt eine variabkendeklaration vor bei 'sdfsdf= ' auch aber der wert dahinter wird nicht mitgenommen
		current = current->next;
	}
}

void	lex_prompt(t_minishell *m)
{
	prompt_to_token(m);
	//ft_printf(Y"TOKENLIST BEFORE ANALYSING:\n"D);
	ft_lstput(&(m->tok_lst), put_token, '\n');
	//analyze_tokens(m);
	//ft_printf(Y"TOKENLIST GANZ AM ENDE:\n"D);
	//ft_lstput(&(m->tok_lst), put_token, '\n');
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
