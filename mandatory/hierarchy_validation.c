/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hierarchy_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:17:19 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/26 19:02:00 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Vaildate the root node to be set on ast
int	vd_null_add(t_btree *ast, t_token *newtok)
{
	if (ast->content == NULL)
	{
		if (ft_strcmp(newtok->type, "Argument") == 0)
		{
			ft_printf(R RT_N_AL D, newtok->type);
			return (0);
		}
		else
			return (1);
	}
	return (0);
}

// Vaildate the parent/child realtionsships to be set in ast
int	check_child_rel(t_token *current, t_token *new)
{
	if (ft_strcmp(current->type, "Redirection") == 0)
	{
		if (current->token > new->token)
			return (0);
	}
	else if (ft_strcmp(current->type, "Pipe") == 0)
	{
		if (current->token == (new->token - 1))
			return (0);
	}
	else if (ft_strcmp(current->type, "Command") == 0)
	{
		if (current->token == (new->token - 1))
			return (0);
	}
	return (1);
}

// Vaildate the sibling/next realtionsships to be set in ast
int	check_next_rel(t_token *current, t_token *new)
{
	if (current->token == new->token)
		return (0);
	return (1);
}

// apply the validation of parent/child sibling/next on ast add
int	vd_tree_add(t_btree *current, char *branch, t_token *newtok)
{
	t_token	*asttok;

	if (current->content != NULL)
	{
		asttok = current->content;
		if (ft_strcmp(branch, "child") == 0)
		{
			if (check_child_rel(asttok, newtok))
			{
				ft_printf(R CT_N_AL D, newtok->type, asttok->type);
				return (0);
			}
		}
		else if (ft_strcmp(branch, "next") == 0)
		{
			if (check_next_rel(asttok, newtok))
			{
				ft_printf(R NT_N_AL D, newtok->type, asttok->type);
				return (0);
			}
		}
	}
	return (1);
}
