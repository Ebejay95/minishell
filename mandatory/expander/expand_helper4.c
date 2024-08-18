/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:09:18 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/16 00:15:42 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	process_token(t_exp *e)
{
	size_t	i;
	char	c;

	i = 0;
	while (i < ft_strlen(e->token->str))
	{
		if (e->token->expmap[i] == '1')
		{
			e->start = i;
			while (i < ft_strlen(e->token->str) && e->token->expmap[i] == '1')
				i++;
			e->end = i;
			handle_unexpanded_part(e);
		}
		else
		{
			e->start = i;
			c = e->token->expmap[i];
			while (i < ft_strlen(e->token->str) && e->token->expmap[i] == c)
				i++;
			e->end = i;
			handle_expanded_part(e);
		}
	}
}
void expand_token(t_minishell *m, t_token *token)
{
    t_exp exp;

    if (!token || !token->str || !token->expmap)
        return;

    exp.result = NULL;
    exp.expmap_result = NULL;
    exp.m = m;
    exp.token = token;
    exp.str = token->str;
    exp.expmap = token->expmap;

    ft_printf("Before init_exp:\n");
    ft_printf("token->str: %p (%s)\n", token->str, token->str);
    ft_printf("token->expmap: %p (%s)\n", token->expmap, token->expmap);

    init_exp(&exp);

    ft_printf("After init_exp:\n");
    ft_printf("exp.result: %p\n", exp.result);
    ft_printf("exp.expmap_result: %p\n", exp.expmap_result);

    if (!exp.result || !exp.expmap_result) {
        free(exp.result);
        free(exp.expmap_result);
        return;
    }

    process_token(&exp);

    ft_printf("After process_token:\n");
    ft_printf("exp.result: %p (%s)\n", exp.result, exp.result);
    ft_printf("exp.expmap_result: %p (%s)\n", exp.expmap_result, exp.expmap_result);

    free(token->str);
    free(token->expmap);

    token->str = exp.result;
    token->expmap = exp.expmap_result;

    ft_printf("After assigning to token:\n");
    ft_printf("token->str: %p (%s)\n", token->str, token->str);
    ft_printf("token->expmap: %p (%s)\n", token->expmap, token->expmap);

}

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

void	expcn(char **res, char **exp_res, char **var_name, char **exit_stats)
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

int	initialize_expansion(t_expand_data *d)
{
	*(d->res) = ft_calloc(1, sizeof(char));
	*(d->exp_res) = ft_calloc(1, sizeof(char));
	if (!*(d->res) || !*(d->exp_res))
	{
		expcn(d->res, d->exp_res,
			d->params->expanded, d->params->expanded_map);
		return (-1);
	}
	return (0);
}
