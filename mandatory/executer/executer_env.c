/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:27:04 by chorst            #+#    #+#             */
/*   Updated: 2024/08/11 20:12:01 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	**own_env(t_envlst *env_lst)
{
	size_t		k;
	char		**env;

	k = 0;
	env = (char **)malloc(sizeof(char *) * (count_list(env_lst) + 1));
	if (!env)
		return (NULL);
	while (env_lst != NULL)
	{
		if (env_lst->value != NULL)
		{
			env[k] = ft_strjoin(ft_strjoin(env_lst->name, "="), env_lst->value);
			if (!env[k])
			{
				while (k > 0)
					free(env[--k]);
				return (free(env), NULL);
			}
			k++;
		}
		env_lst = env_lst->next;
	}
	env[k] = NULL;
	return (env);
}
