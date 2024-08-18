/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 00:09:18 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/19 01:14:55 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void process_token(t_exp *e)
{
    ft_printf("DEBUG: Entering process_token\n");
    ft_printf("DEBUG: Token str: %s\n", e->token->str);
    ft_printf("DEBUG: Token expmap: %s\n", e->token->expmap);

    size_t i;
    char c;

    i = 0;
    while (i < ft_strlen(e->token->str))
    {
        ft_printf("DEBUG: Processing character at index %zu: %c\n", i, e->token->str[i]);
        if (e->token->expmap[i] == '1')
        {
            ft_printf("DEBUG: Handling unexpanded part\n");
            e->start = i;
            while (i < ft_strlen(e->token->str) && e->token->expmap[i] == '1')
                i++;
            e->end = i;
            handle_unexpanded_part(e);
        }
        else
        {
            ft_printf("DEBUG: Handling expanded part\n");
            e->start = i;
            c = e->token->expmap[i];
            while (i < ft_strlen(e->token->str) && e->token->expmap[i] == c)
                i++;
            e->end = i;
            handle_expanded_part(e);
        }
        ft_printf("DEBUG: After processing, i = %zu\n", i);
    }
    ft_printf("DEBUG: Exiting process_token\n");
}
void expand_token(t_minishell *m, t_token *token)
{
    ft_printf("DEBUG: Entering expand_token for token: %s\n", token->str);
    ft_printf("DEBUG: Token expmap: %s\n", token->expmap);

    t_exp exp;

    ft_printf("DEBUG: Initializing exp structure\n");
    exp.result = NULL;
    exp.expmap_result = NULL;
    exp.m = m;
    exp.token = token;
    exp.str = token->str;
    exp.expmap = token->expmap;

    ft_printf("DEBUG: Before init_exp\n");
    init_exp(&exp);
    ft_printf("DEBUG: After init_exp\n");

    if (!exp.result || !exp.expmap_result) {
        ft_printf("DEBUG: exp.result or exp.expmap_result is NULL, returning\n");
        free(exp.result);
        free(exp.expmap_result);
        return;
    }

    ft_printf("DEBUG: Before process_token\n");
    process_token(&exp);
    ft_printf("DEBUG: After process_token\n");

    ft_printf("DEBUG: Before freeing and reassigning token members\n");
    free(token->str);
    free(token->expmap);

    token->str = exp.result;
    token->expmap = exp.expmap_result;

    ft_printf("DEBUG: Exiting expand_token. New token str: %s\n", token->str);
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
