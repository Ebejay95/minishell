/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/21 14:31:25 by jeberle          ###   ########.fr       */
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