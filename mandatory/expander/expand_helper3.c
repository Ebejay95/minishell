/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:09:18 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/16 00:14:59 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int	allocate_expmap_temp_memory(t_exp *exp, t_exp_data *data)
{
	size_t	len;

	len = ft_strlen(exp->expmap_result) + ft_strlen(data->expanded_map) + 1;
	data->expmap_temp = ft_calloc(len, sizeof(char));
	if (!data->expmap_temp)
		return (-1);
	return (0);
}

void	update_result(t_exp *exp, t_exp_data *data)
{
	ft_strlcpy(data->temp, exp->result, ft_strlen(exp->result) + 1);
	free(exp->result);
	exp->result = data->temp;
	ft_strcat(exp->result, data->expanded);
}

void	update_expmap_result(t_exp *exp, t_exp_data *data)
{
	size_t	len;

	len = ft_strlen(exp->expmap_result) + 1;
	ft_strlcpy(data->expmap_temp, exp->expmap_result, len);
	free(exp->expmap_result);
	exp->expmap_result = data->expmap_temp;
	ft_strcat(exp->expmap_result, data->expanded_map);
}

void	cleanup_expansion_data(t_exp_data *data)
{
	free(data->expanded);
	free(data->expanded_map);
}

void handle_expanded_part(t_exp *exp)
{
    t_exp_data data;
    t_exp_p params;
    t_expand_ctx *ctx;

    ctx = NULL;
    init_exp_data(&data);
    setup_expand_params(exp, &params, &data);
	    ft_printf("In handle_expanded_part: data.temp=%p, data.expmap_temp=%p\n", data.temp, data.expmap_temp);

    expand(&params, ctx);
    if (data.expanded && data.expanded_map) {
        if (allocate_temp_memory(exp, &data) == -1) {
            cleanup_expansion_data(&data);
            return;
        }
        update_result(exp, &data);
        if (allocate_expmap_temp_memory(exp, &data) == -1) {
            free(data.temp);
            cleanup_expansion_data(&data);
            return;
        }
        update_expmap_result(exp, &data);
    }
    cleanup_expansion_data(&data);
	    ft_printf("After cleanup: data.temp=%p, data.expmap_temp=%p\n", data.temp, data.expmap_temp);

}
