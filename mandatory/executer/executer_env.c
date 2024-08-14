/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:27:04 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 13:00:25 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	**own_env(t_envlst *env_lst)
{
	size_t		k;
	char		**env;
	char		*tmp;

	k = 0;
	env = (char **)malloc(sizeof(char *) * (count_list(env_lst) + 1));
	if (!env)
		return (NULL);
	while (env_lst != NULL)
	{
		if (env_lst->value != NULL)
		{
			tmp = ft_strjoin(env_lst->name, "=");
			env[k] = ft_strjoin(tmp, env_lst->value);
			if (!env[k])
			{
				while (k > 0)
					free(env[--k]);
				free(env);
				return (NULL);
			}
			free(tmp);
			k++;
		}
		env_lst = env_lst->next;
	}
	env[k] = NULL;
	return (env);
}
