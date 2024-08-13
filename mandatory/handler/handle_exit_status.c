/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:08:08 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 18:40:25 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	handle_exit_status(t_minishell *m, char **res, char **expmap_res)
{
	char	*exit_status_str;
	char	*temp;
	char	*expmap_temp;

	exit_status_str = ft_itoa(m->last_exitcode);
	if (!exit_status_str)
	{
		exp_cln(*res, *expmap_res, NULL, NULL);
		return (-1);
	}
	temp = ft_realloc(*res, ft_strlen(*res) + ft_strlen(exit_status_str) + 1);
	expmap_temp = ft_realloc(*expmap_res,
			ft_strlen(*expmap_res) + ft_strlen(exit_status_str) + 1);
	if (!temp || !expmap_temp)
	{
		exp_cln(*res, *expmap_res, NULL, exit_status_str);
		return (-1);
	}
	*res = temp;
	*expmap_res = expmap_temp;
	ft_strcat(*res, exit_status_str);
	ft_strfillcat(*expmap_res, exit_status_str,
		(*expmap_res)[ft_strlen(*expmap_res) - 1]);
	free(exit_status_str);
	return (2);
}
