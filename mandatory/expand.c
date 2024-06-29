/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/28 11:05:06 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// bash-5.2$ $HOME
// bash: /Users/jeberle: Is a directory
// bash-5.2$ echo $HOME
// /Users/jeberle
// bash-5.2$ echo "$HOME"  
// /Users/jeberle
// bash-5.2$ echo '$HOME'
// $HOME
// bash-5.2$ echo "'$HOME'"
// '/Users/jeberle'
// bash-5.2$ echo '"$HOME"'
// "$HOME"
// bash-5.2$ echo "cat './include/minishell.h' | grep #"
// cat './include/minishell.h' | grep #
// bash-5.2$ echo "cat $HOME"
// cat /Users/jeberle
// bash-5.2$ echo "$HOME"
// /Users/jeberle
// bash-5.2$ echo 'cat $HOME'
// cat $HOME
// bash-5.2$
// bash-5.2$ echo $HOME
// /Users/jeberle
// bash-5.2$ echo "$HOME"  
// /Users/jeberle
// bash-5.2$ echo '$HOME'
// $HOME
// bash-5.2$ echo "'$HOME'"
// '/Users/jeberle'
// bash-5.2$ echo '"$HOME"'
// "$HOME"
// bash-5.2$ echo "'"$HOME"'"
// '/Users/jeberle'
// bash-5.2$ echo '"'$HOME'"'
// "/Users/jeberle"
// bash-5.2$ echo '"'"$HOME"'"'
// "/Users/jeberle"
// bash-5.2$ echo "'"'$HOME'"'"
// '$HOME'
// bash-5.2$ echo "'"'"$HOME"'"'"
// '"$HOME"'
// bash-5.2$ echo '"'"'$HOME'"'"'
// "'/Users/jeberle'"
// bash-5.2$ echo '"'"'"$HOME"'"'"'
// "'/Users/jeberle'"
// bash-5.2$ echo "'"'"'$HOME'"'"'"
// '"/Users/jeberle"'
// bash-5.2$ echo "'"'"'"$HOME"'"'"'"
// '"/Users/jeberle"'
// bash-5.2$ echo '"'"'"'$HOME'"'"'"'
// "'$HOME'"
// bash-5.2$ echo '"'"'"'"$HOME"'"'"'"'
// "'"$HOME"'"
// bash-5.2$ echo "'"'"'"'$HOME'"'"'"'"
// '"'/Users/jeberle'"'
// bash-5.2$ echo "'"'"'"'"$HOME"'"'"'"'"
// '"'/Users/jeberle'"'
// bash-5.2$ echo '"'"'"'"'$HOME'"'"'"'"'
// "'"/Users/jeberle"'"
// bash-5.2$ 

//int	can_expand(t_lexer lexer, int index, t_expsegment *exp_seg)
//{
//	t_segment	*watch;
//	char		*varname;
//
//	varname = NULL;
//	watch = find_seg_by_start(lexer.variable_buffer, index);
//	if (watch)
//		varname = watch->str;
//	if (varname && varname[0] == '$')
//	{
//		exp_seg->str = ft_strdup(varname);
//		exp_seg->start = watch->start;
//		exp_seg->end = watch->end;
//		varname++;
//		exp_seg->value = ft_getenv(varname);
//		return (1);
//	}
//	return (0);
//}


// char *expand_variables(char *prompt, int watchstart, int watchend) {
//     int length = ft_strlen(prompt);
//     char *result = malloc(length + 1);
//     char *result_ptr = result;

//     for (int i = 0; i < watchstart; i++) {
//         *result_ptr++ = prompt[i];
//     }

//     for (int i = watchstart; i <= watchend && i < length; i++) {
//         if (prompt[i] == '$') {
//             // Found the start of a variable
//             i++;
//             char var_name[256];
//             int var_len = 0;

//             // Collect the variable name
//             while (i <= watchend && i < length && (isalnum(prompt[i]) || prompt[i] == '_')) {
//                 var_name[var_len++] = prompt[i++];
//             }
//             var_name[var_len] = '\0';
//             i--;

//             // Get the variable value
//             char *var_value = ft_getenv(var_name);

//             // Copy the variable value to the result
//             while (*var_value) {
//                 *result_ptr++ = *var_value++;
//             }
//         } else {
//             *result_ptr++ = prompt[i];
//         }
//     }

//     for (int i = watchend + 1; i < length; i++) {
//         *result_ptr++ = prompt[i];
//     }

//     *result_ptr = '\0';
//     return result;
// }

// void ft_expand(char **prompt, t_lexer lexer, int start_index)
// {
//     char *expanded;
//     t_segment *watch;

//     expanded = NULL;

//     if (prompt == NULL || *prompt == NULL)
//         return;

//     watch = find_seg_by_start(lexer.variable_buffer, start_index);
// 	ft_printf("%p\n", watch);
//     if (watch != NULL)
//     {
// 		ft_printf("%i %i\n", watch->start, watch->end);
//         expanded = expand_variables(*prompt, watch->start, watch->end);
//         if (expanded != *prompt)
//         {
//             free(*prompt);
//             *prompt = expanded;
//         }
//     }
// }
