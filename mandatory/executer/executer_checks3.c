/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_checks3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:58:30 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 15:31:13 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	check_six(t_minishell *m, enum e_toktype end, enum e_toktype conty)
{
	if (!m->leave && end == REDIRECTION && conty == REDIRECTION)
		return (1);
	return (0);
}
