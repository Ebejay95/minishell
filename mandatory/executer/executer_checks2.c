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

int	check_one(t_minishell *m, char *lty, char *conty)
{
	if (!m->leave && !ft_strcmp(lty, PIP) && !ft_strcmp(conty, PIP))
		return (1);
	return (0);
}

int	check_two(t_minishell *m, char *conty, char *end)
{
	if (!m->leave && ft_strcmp(conty, PIP) == 0 && end == NULL)
		return (1);
	return (0);
}

int	check_three(t_minishell *m, t_list *curnext, char *conty)
{
	if (!m->leave && curnext == NULL && !ft_strcmp(conty, PIP))
		return (1);
	return (0);
}

int	check_four(t_minishell *m, char *conty, char *end)
{
	if (!m->leave && !ft_strcmp(conty, PIP) && !ft_strcmp(end, RDRC))
		return (1);
	return (0);
}

int	check_five(t_minishell *m, t_list *curnext, char *conty)
{
	if (!m->leave && curnext == NULL && !ft_strcmp(conty, RDRC))
		return (1);
	return (0);
}
