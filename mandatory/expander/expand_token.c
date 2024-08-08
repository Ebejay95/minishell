/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:09:22 by chorst            #+#    #+#             */
/*   Updated: 2024/08/07 16:41:40 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

void	expand_token(t_minishell *m, t_token *token)
{
	char	*temp;
	char	*expmap_temp;
	size_t	len;
	size_t	i;
	size_t	start;
	char	current_mode;
	char	*expanded;
	char	*expanded_map;
	char	*result;
	char	*expmap_result;

	expanded = NULL;
	expanded_map = NULL;
	if (!token || !token->str || !token->expmap)
		return ;
	len = strlen(token->str);
	result = malloc(sizeof(char));
	if (!result)
		return ;
	result[0] = '\0';
	expmap_result = malloc(sizeof(char));
	if (!expmap_result)
		return ;
	expmap_result[0] = '\0';
	i = 0;
	while (i < len)
	{
		if (token->expmap[i] == '1')
		{
			start = i;
			while (i < len && token->expmap[i] == '1')
				i++;
			temp = realloc(result, strlen(result) + (i - start) + 1);
			if (!temp)
			{
				free(result);
				return ;
			}
			result = temp;
			strncat(result, &token->str[start], i - start);
		}
		else
		{
			start = i;
			current_mode = token->expmap[i];
			while (i < len && token->expmap[i] == current_mode)
				i++;
			expand(m, &expanded, &expanded_map, &token->str[start], &token->expmap[start], 0, i - start);
			if (expanded && expanded_map)
			{
				temp = ft_realloc(result, ft_strlen(result) + ft_strlen(expanded) + 1);
				if (!temp)
				{
					free(result);
					free(expanded);
					return ;
				}
				result = temp;
				ft_strcat(result, expanded);
				expmap_temp = ft_realloc(expmap_result, ft_strlen(expmap_result) + ft_strlen(expanded_map) + 1);
				if (!expmap_temp)
				{
					free(expmap_result);
					free(expanded_map);
					return ;
				}
				expmap_result = expmap_temp;
				ft_strcat(expmap_result, expanded_map);
			}
		}
	}
	free(token->str);
	token->str = result;
	token->expmap = expmap_result;
}
