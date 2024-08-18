/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:09:18 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 01:16:56 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void init_exp(t_exp *exp)
{
    ft_printf("DEBUG: Entering init_exp\n");

    if (exp->result)
    {
        ft_printf("DEBUG: Freeing existing exp->result\n");
        free(exp->result);
        exp->result = NULL;
    }
    if (exp->expmap_result)
    {
        ft_printf("DEBUG: Freeing existing exp->expmap_result\n");
        free(exp->expmap_result);
        exp->expmap_result = NULL;
    }

    ft_printf("DEBUG: Allocating memory for exp->result\n");
    exp->result = ft_calloc(1, sizeof(char));
    ft_printf("DEBUG: Allocating memory for exp->expmap_result\n");
    exp->expmap_result = ft_calloc(1, sizeof(char));

    if (!exp->result || !exp->expmap_result)
    {
        ft_printf("DEBUG: Memory allocation failed in init_exp\n");
    }
    else
    {
        ft_printf("DEBUG: Memory allocation successful in init_exp\n");
    }

    ft_printf("DEBUG: Exiting init_exp\n");
}

void	handle_unexpanded_part(t_exp *exp)
{
	char	*temp;
	size_t	result_len;
	size_t	part_len;

	result_len = ft_strlen(exp->result);
	part_len = exp->end - exp->start;
	temp = ft_calloc(result_len + part_len + 1, sizeof(char));
	if (!temp)
		return ;
	ft_strlcpy(temp, exp->result, result_len + 1);
	free(exp->result);
	exp->result = temp;
	ft_strncat(exp->result, &exp->str[exp->start], part_len);
}

void	init_exp_data(t_exp_data *data)
{
	data->expanded = NULL;
	data->expanded_map = NULL;
	data->temp = NULL;
	data->expmap_temp = NULL;
}

void	setup_expand_params(t_exp *exp, t_exp_p *params, t_exp_data *data)
{
	params->m = exp->m;
	params->expanded = &data->expanded;
	params->expanded_map = &data->expanded_map;
	params->str = &exp->str[exp->start];
	params->expmap = &exp->expmap[exp->start];
	params->start = 0;
	params->end = exp->end - exp->start;
}

int	allocate_temp_memory(t_exp *e, t_exp_data *d)
{
	size_t	len;

	len = ft_strlen(e->result) + ft_strlen(d->expanded) + 1;
	d->temp = ft_calloc(len, sizeof(char));
	if (!d->temp)
		return (-1);
	return (0);
}
