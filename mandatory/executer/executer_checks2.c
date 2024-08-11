/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:58:30 by chorst            #+#    #+#             */
/*   Updated: 2024/08/11 20:40:49 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	check_one(t_minishell *m, char *lty, char *conty)
{
	if (!m->leave && !ft_strcmp(lty, text(8)) && !ft_strcmp(conty, text(8)))
		return (1);
	return (0);
}

int	check_two(t_minishell *m, char *conty, char *end)
{
	if (!m->leave && ft_strcmp(conty, text(8)) == 0 && end == NULL)
		return (1);
	return (0);
}

int	check_three(t_minishell *m, t_list *curnext, char *conty)
{
	if (!m->leave && curnext == NULL && !ft_strcmp(conty, text(8)))
		return (1);
	return (0);
}

int	check_four(t_minishell *m, char *conty, char *end)
{
	if (!m->leave && !ft_strcmp(conty, text(8)) && !ft_strcmp(end, text(7)))
		return (1);
	return (0);
}

int	check_five(t_minishell *m, t_list *curnext, char *conty)
{
	if (!m->leave && curnext == NULL && !ft_strcmp(conty, text(7)))
		return (1);
	return (0);
}
