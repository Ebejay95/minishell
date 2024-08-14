/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/14 15:28:21 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	expmapcheck(char *expmap, const char *str, int i, int escaped)
{
	if (expmap[i + 1] != '\0' && str[i + 1] != '\0' && !escaped)
	{
		if (expmap[i] == '0'
			&& expmap[i + 1] == '2'
			&& str[i + 1] == '\"')
			return (2);
		if (expmap[i + 2] != '\0'
			&& str[i + 2] != '\0'
			&& expmap[i + 1] == '0'
			&& expmap[i + 2] == '2'
			&& str[i + 1] == '\"')
			return (2);
		else if ((expmap[i] == '0' || expmap[i] == '2')
			&& expmap[i] == expmap[i + 1])
			return (1);
	}
	return (0);
}

void	exp_cln(char **res, char **exp_res, char **var_name, char **exit_stats)
{
	if (res && *res)
	{
		free(*res);
		*res = NULL;
	}
	if (exp_res && *exp_res)
	{
		free(*exp_res);
		*exp_res = NULL;
	}
	if (var_name && *var_name)
	{
		free(*var_name);
		*var_name = NULL;
	}
	if (exit_stats && *exit_stats)
	{
		free(*exit_stats);
		*exit_stats = NULL;
	}
}

void	expand(t_minishell *m, char **expanded, char **expanded_map, const char *str, char *expmap, size_t start, size_t end)
{
	char	*res;
	char	*exp_res;
	size_t	i;
	int		escaped;
	int		ret;

	res = ft_calloc(1, sizeof(char));
	exp_res = ft_calloc(1, sizeof(char));
	if (!res || !exp_res)
	{
		exp_cln(&res, &exp_res, NULL, NULL);
		*expanded = NULL;
		*expanded_map = NULL;
		return ;
	}
	i = start;
	escaped = 0;
	while (i < end)
	{
		if (expmap[i] == 'S')
		{
			i++;
			continue ;
		}
		if (str[i] == '\\')
		{
			ret = handle_escape(&res, &exp_res, &escaped);
			if (ret == -1)
			{
				*expanded = NULL;
				*expanded_map = NULL;
				return ;
			}
			if (ret > 0)
			{
				i += ret;
				continue ;
			}
		}
		if (str[i] == '$' && str[i + 1] == '?' && expmapcheck(expmap, str, i, escaped) == 1)
		{
			ret = handle_exit_status(m, &res, &exp_res);
			if (ret == -1)
			{
				*expanded = NULL;
				*expanded_map = NULL;
				return ;
			}
			i += ret;
		}
		else if (str[i] == '$' && expmapcheck(expmap, str, i, escaped) == 2)
			i += 2;
		else if (str[i] == '$' && expmapcheck(expmap, str, i, escaped) == 1)
		{
			ret = handle_variable(m, &res, &exp_res, str, expmap, &i);
			if (ret == -1)
			{
				*expanded = NULL;
				*expanded_map = NULL;
				return ;
			}
		}
		else
		{
			if (str[i] != '"')
			{
				char *temp = ft_calloc(ft_strlen(res) + 2, sizeof(char));
				char *exp_tmp = ft_calloc(ft_strlen(exp_res) + 2, sizeof(char));
				if (!temp || !exp_tmp)
				{
					exp_cln(&res, &exp_res, NULL, NULL);
					*expanded = NULL;
					*expanded_map = NULL;
					return ;
				}
				ft_strlcpy(temp, res, ft_strlen(res) + 1);
				ft_strlcpy(exp_tmp, exp_res, ft_strlen(exp_res) + 1);
				free(res);
				free(exp_res);
				res = temp;
				exp_res = exp_tmp;
				ft_strncat(res, &str[i], 1);
				ft_strncat(exp_res, &expmap[i], 1);
			}
			i++;
		}
	}
	*expanded = res;
	*expanded_map = exp_res;
}
