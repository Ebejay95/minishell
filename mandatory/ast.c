/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:31:17 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/26 19:02:40 by jeberle          ###   ########.fr       */
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
// hadnling to add nodes to ast at postiion cur as child or sibling by token
void	ast_add(t_btree **ast, t_btree *cur, char *branch, t_token *tok)
{
	t_btree	*new;
	t_token	*asttok;

	new = NULL;
	if (cur == NULL)
	{
		if (ft_strcmp(branch, "child") == 0)
			ft_printf(R AD_C_ON_N D, tok->type);
		else if (ft_strcmp(branch, "next") == 0)
			ft_printf(R AD_N_ON_N D, tok->type);
		return ;
	}
	if (ft_strcmp(branch, "next") != 0 && ft_strcmp(branch, "child") != 0)
	{
		ft_printf(R B_NO_DEF D);
		return ;
	}
	if (vd_null_add(cur, tok))
	{
		new = ft_btreenew((void *)tok);
		if (cur == *ast)
		{
			*ast = new;
		}
		else
		{
			cur = new;
		}
	}
	else if (vd_tree_add(cur, branch, tok) && cur->content != NULL)
	{
		new = ft_btreenew((void *)tok);
		if (ft_strcmp(branch, "next") == 0)
			ft_btreeadd_next(cur, new);
		if (ft_strcmp(branch, "child") == 0)
			ft_btreeadd_child(cur, new);
	}
	else if (cur->content != NULL)
	{
		asttok = cur->content;
		if (ft_strcmp(branch, "child") == 0)
			ft_printf(R AD_C_N_AL D, tok->type, asttok->type);
		else if (ft_strcmp(branch, "next") == 0)
			ft_printf(R AD_N_N_AL D, tok->type, asttok->type);
		return ;
	}
}
