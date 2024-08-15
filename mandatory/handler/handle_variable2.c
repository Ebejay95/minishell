/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:12:31 by chorst            #+#    #+#             */
/*   Updated: 2024/08/16 00:12:33 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	handle_no_var_name(t_expand_ctx *ctx)
{
	char	*temp;
	char	*expmap_temp;
	size_t	result_len;

	result_len = ft_strlen(*(ctx->result));
	temp = ft_calloc(result_len + 2, sizeof(char));
	expmap_temp = ft_calloc(result_len + 2, sizeof(char));
	if (!temp || !expmap_temp)
	{
		free(temp);
		free(expmap_temp);
		expcn(ctx->result, ctx->expmap_result, NULL, NULL);
		return (-1);
	}
	ft_strlcpy(temp, *(ctx->result), result_len + 1);
	ft_strlcpy(expmap_temp, *(ctx->expmap_result), result_len + 1);
	temp[result_len] = ctx->str[ctx->var_start];
	expmap_temp[result_len] = ctx->expmap[ctx->var_start];
	free(*(ctx->result));
	free(*(ctx->expmap_result));
	*(ctx->result) = temp;
	*(ctx->expmap_result) = expmap_temp;
	*(ctx->i) = ctx->var_start + 1;
	return (0);
}

int	allocate_memory(t_memory_info *m)
{
	m->temp = ft_calloc(m->result_len + m->value_len + 1, sizeof(char));
	m->expmap_temp = ft_calloc(m->expmap_len + m->value_len + 1, sizeof(char));
	if (!m->temp || !m->expmap_temp)
	{
		free(m->temp);
		free(m->expmap_temp);
		return (-1);
	}
	return (0);
}
