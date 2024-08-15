/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:12:31 by chorst            #+#    #+#             */
/*   Updated: 2024/08/16 00:11:53 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	copy_and_concatenate(t_expand_ctx *ctx, t_memory_info *m, char *v)
{
	ft_strlcpy(m->temp, *(ctx->result), m->result_len + 1);
	ft_strlcat(m->temp, v, m->result_len + m->value_len + 1);
	ft_strlcpy(m->expmap_temp, *(ctx->expmap_result), m->expmap_len + 1);
}

void	update_expmap(t_expand_ctx *ctx, char *expmap_temp, char *var_value)
{
	if (ft_strcontains(ctx->expmap, '2'))
	{
		ft_strfillcat(expmap_temp, var_value, 'X');
	}
	else
	{
		ft_strfillcat(expmap_temp, var_value, 'E');
	}
}

void	update_and_free(t_expand_ctx *ctx, t_memory_info *mem_info)
{
	free(*(ctx->result));
	free(*(ctx->expmap_result));
	*(ctx->result) = mem_info->temp;
	*(ctx->expmap_result) = mem_info->expmap_temp;
}

int	handle_var_value(t_expand_ctx *ctx, char *var_value)
{
	t_memory_info	mem_info;

	mem_info.result_len = ft_strlen(*(ctx->result));
	mem_info.expmap_len = ft_strlen(*(ctx->expmap_result));
	mem_info.value_len = ft_strlen(var_value);
	if (allocate_memory(&mem_info) == -1)
	{
		expcn(ctx->result, ctx->expmap_result, NULL, NULL);
		return (-1);
	}
	copy_and_concatenate(ctx, &mem_info, var_value);
	update_expmap(ctx, mem_info.expmap_temp, var_value);
	update_and_free(ctx, &mem_info);
	return (0);
}

int	handle_variable(t_expand_ctx *ctx)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name_exp(ctx->str, ctx->expmap, ctx->i);
	if (!var_name)
	{
		return (handle_no_var_name(ctx));
	}
	var_value = my_getenv(var_name, ctx->m->env_list);
	if (var_value)
	{
		if (handle_var_value(ctx, var_value) == -1)
		{
			free(var_name);
			return (-1);
		}
	}
	free(var_name);
	return (0);
}
