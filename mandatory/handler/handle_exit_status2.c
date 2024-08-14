/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_status2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:08:08 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 12:50:16 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	are_exitinputs_valid(t_minishell *m, char **res, char **expmap_res)
{
	return (m && res && expmap_res && *res && *expmap_res);
}

char	*get_exit_status_str(t_minishell *m)
{
	return (ft_itoa(m->last_exitcode));
}
