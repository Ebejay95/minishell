/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:23:03 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/16 00:16:01 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	expand(t_exp_p *p, t_expand_ctx *ctx)
{
	char				*res;
	char				*exp_res;
	size_t				i;
	int					escaped;
	t_expand_data		data;

	i = p->start;
	escaped = 0;
	data = (t_expand_data){&res, &exp_res, p->str, p->expmap, &i, &escaped, p};
	if (initialize_expansion(&data) == -1)
		return ;
	ctx = &((t_expand_ctx){p->m, &res, &exp_res, p->str, p->expmap, &i, 0});
	while (i < p->end)
	{
		if (process_character(&data, ctx) == -1)
			return ;
	}
	finalize_expansion(&data);
}
