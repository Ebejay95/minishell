/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:08:08 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 15:36:44 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	alloc_exit_buf(char **temp, char **temp2, size_t res_len, size_t exit_len)
{
	*temp = ft_calloc(res_len + exit_len + 1, sizeof(char));
	*temp2 = ft_calloc(res_len + exit_len + 1, sizeof(char));
	if (!*temp || !*temp2)
	{
		free(*temp);
		return (0);
	}
	return (1);
}

void	add_str(char *dst, const char *src1, const char *src2, size_t len_total)
{
	size_t	len1;

	len1 = ft_strlen(src1);
	ft_strlcpy(dst, src1, len1 + 1);
	ft_strlcat(dst, src2, len_total + 1);
}

int	add_expmap(char *dst, const char *src, const char *exit_str, char def_char)
{
	size_t	res_len;
	char	fill_char;

	res_len = ft_strlen(src);
	ft_strlcpy(dst, src, res_len + 1);
	if (res_len > 0)
		fill_char = src[res_len - 1];
	else
		fill_char = def_char;
	ft_strfillcat(dst, exit_str, fill_char);
	return (0);
}

int	add_exit_status(char **res, char **expmap_res, const char *exit)
{
	t_temps	t;
	size_t	res_len;
	size_t	exit_len;

	res_len = ft_strlen(*res);
	exit_len = ft_strlen(exit);
	if (!alloc_exit_buf(&t.temp, &t.expmap, res_len, exit_len))
	{
		exp_cln(res, expmap_res, NULL, NULL);
		return (0);
	}
	add_str(t.temp, *res, exit, exit_len);
	add_expmap(t.expmap, *expmap_res, exit, res_len);
	free(*res);
	free(*expmap_res);
	*res = t.temp;
	*expmap_res = t.expmap;
	return (1);
}

int	handle_exit_status(t_minishell *m, char **res, char **expmap_res)
{
	char	*exit_status_str;

	if (!are_exitinputs_valid(m, res, expmap_res))
		return (-1);
	exit_status_str = get_exit_status_str(m);
	if (!exit_status_str)
	{
		exp_cln(res, expmap_res, NULL, NULL);
		return (-1);
	}
	if (!add_exit_status(res, expmap_res, exit_status_str))
	{
		free(exit_status_str);
		return (-1);
	}
	free(exit_status_str);
	return (2);
}
