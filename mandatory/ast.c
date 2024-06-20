/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:31:17 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/20 12:09:24 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

//	t_token *redirection = create_token(REDIRECTION);
//	t_token *pipe = create_token(PIPE);
//	t_token *command = create_token(COMMAND);
//	t_token *argument = create_token(ARGUMENT);
//
//	create root ast node | add to existing node
//	ast_add(&ast, ast, "next", redirection);
//
//	adding a same level node (sibling|next)
//	ast_add(&ast, ast, "next", redirection);
//
//	adding a child node
//	ast_add(&ast, ast, "child", pipe);
//
// hadnling to add nodes to ast at postiion current as child or sibling by token
void	ast_add(t_btree **ast, t_btree *current, char *branch, t_token *newtok)
{
	t_btree	*new;
	t_token	*asttok;

	new = NULL;
	if (current == NULL)
	{
		if (ft_strcmp(branch, "child") == 0)
			ft_printf(AD_C_ON_N, newtok->type);
		else if (ft_strcmp(branch, "next") == 0)
			ft_printf(AD_N_ON_N, newtok->type);
		return ;
	}
	if (ft_strcmp(branch, "next") != 0 && ft_strcmp(branch, "child") != 0)
	{
		ft_printf(B_NO_DEF);
		return ;
	}
	if (vd_null_add(current, newtok))
	{
		new = ft_btreenew((void *)newtok);
		if (current == *ast)
		{
			*ast = new;
		}
		else
		{
			current = new;
		}
	}
	else if (vd_tree_add(current, branch, newtok) && current->content != NULL)
	{
		new = ft_btreenew((void *)newtok);
		if (ft_strcmp(branch, "next") == 0)
			ft_btreeadd_next(current, new);
		if (ft_strcmp(branch, "child") == 0)
			ft_btreeadd_child(current, new);
	}
	else if (current->content != NULL)
	{
		asttok = current->content;
		if (ft_strcmp(branch, "child") == 0)
			ft_printf(AD_C_N_AL, newtok->type, asttok->type);
		else if (ft_strcmp(branch, "next") == 0)
			ft_printf(AD_N_N_AL, newtok->type, asttok->type);
		return ;
	}
}
