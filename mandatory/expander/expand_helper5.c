/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:09:18 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 07:22:01 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	handle_dollar_sign(t_expand_data *data, t_expand_ctx *ctx)
{
	int	ret;
	int	check;

	check = expmapcheck(data->expmap, data->str, *(data->i), *(data->escaped));
	if (data->str[*(data->i) + 1] == '?' && check == 1)
	{
		ret = handle_exit_status(data->params->m, data->res, data->exp_res);
		if (ret == -1)
			return (-1);
		*(data->i) += ret;
	}
	else if (check == 2)
		*(data->i) += 2;
	else if (check == 1)
	{
		ctx->var_start = *(data->i);
		ret = handle_variable(ctx);
		if (ret == -1)
			return (-1);
	}
	return (0);
}

int	handle_escape_character(t_expand_data *d)
{
	int	ret;

	ret = handle_escape(d->res, d->exp_res, d->escaped);
	if (ret == -1)
	{
		expcn(d->res, d->exp_res, d->params->expanded, d->params->expanded_map);
		return (-1);
	}
	if (ret > 0)
	{
		*(d->i) += ret;
	}
	return (0);
}

int	append_regular_character(t_expand_data *data)
{
	char	*temp;
	char	*exp_tmp;

	temp = ft_calloc(ft_strlen(*(data->res)) + 2, sizeof(char));
	exp_tmp = ft_calloc(ft_strlen(*(data->exp_res)) + 2, sizeof(char));
	if (!temp || !exp_tmp)
	{
		expcn(data->res, data->exp_res, NULL, NULL);
		return (-1);
	}
	ft_strlcpy(temp, *(data->res), ft_strlen(*(data->res)) + 1);
	ft_strlcpy(exp_tmp, *(data->exp_res), ft_strlen(*(data->exp_res)) + 1);
	free(*(data->res));
	free(*(data->exp_res));
	*(data->res) = temp;
	*(data->exp_res) = exp_tmp;
	ft_strncat(*(data->res), &(data->str[*(data->i)]), 1);
	ft_strncat(*(data->exp_res), &(data->expmap[*(data->i)]), 1);
	return (0);
}

int	process_character(t_expand_data *data, t_expand_ctx *ctx)
{
	int	ret;

	if (data->expmap[*(data->i)] == 'S')
	{
		(*(data->i))++;
		return (0);
	}
	if (data->str[*(data->i)] == '\\')
		return (handle_escape_character(data));
	ret = expmapcheck(data->expmap, data->str, *(data->i), *(data->escaped));
	if (data->str[*(data->i)] == '$' && ret != 0)
		return (handle_dollar_sign(data, ctx));
	if (data->str[*(data->i)] != '"')
	{
		ret = append_regular_character(data);
		if (ret == 0)
			(*(data->i))++;
		return (ret);
	}
	(*(data->i))++;
	return (0);
}

void	finalize_expansion(t_expand_data *data)
{
	*(data->params->expanded) = *(data->res);
	*(data->params->expanded_map) = *(data->exp_res);
}
