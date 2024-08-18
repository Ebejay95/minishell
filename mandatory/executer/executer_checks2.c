/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:58:30 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 15:31:02 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	check_one(t_minishell *m, enum e_toktype lty, enum e_toktype conty)
{
	if (!m->leave && lty == PIPE && conty == PIPE)
		return (1);
	return (0);
}

int	check_two(t_minishell *m, enum e_toktype conty, enum e_toktype end)
{
	if (!m->leave && conty == PIPE && end == UNSET)
		return (1);
	return (0);
}

int	check_three(t_minishell *m, t_list *curnext, enum e_toktype conty)
{
	if (!m->leave && curnext == NULL && conty == PIPE)
		return (1);
	return (0);
}

int	check_four(t_minishell *m, enum e_toktype conty, enum e_toktype end)
{
	if (!m->leave && conty == PIPE && end == REDIRECTION)
		return (1);
	return (0);
}

int	check_five(t_minishell *m, t_list *curnext, enum e_toktype conty)
{
	if (!m->leave && curnext == NULL && conty == REDIRECTION)
		return (1);
	return (0);
}
