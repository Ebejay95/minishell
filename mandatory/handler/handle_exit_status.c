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
	size_t	res_len, exit_len;

	if (!m || !res || !expmap_res || !*res || !*expmap_res)
		return (-1);

	exit_status_str = ft_itoa(m->last_exitcode);
	if (!exit_status_str)
	{
		exp_cln(res, expmap_res, NULL, NULL);
		return (-1);
	}

	res_len = ft_strlen(*res);
	exit_len = ft_strlen(exit_status_str);

	temp = ft_calloc(res_len + exit_len + 1, sizeof(char));
	expmap_temp = ft_calloc(res_len + exit_len + 1, sizeof(char));
	if (!temp || !expmap_temp)
	{
		free(temp);
		free(exit_status_str);
		exp_cln(res, expmap_res, NULL, NULL);
		return (-1);
	}

	ft_strlcpy(temp, *res, res_len + 1);
	ft_strlcat(temp, exit_status_str, res_len + exit_len + 1);

	ft_strlcpy(expmap_temp, *expmap_res, res_len + 1);
	ft_strfillcat(expmap_temp, exit_status_str, res_len > 0 ? (*expmap_res)[res_len - 1] : 'E');
	free(*res);
	free(*expmap_res);
	*res = temp;
	*expmap_res = expmap_temp;
	free(exit_status_str);
	return (2);
}
