/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:27:04 by chorst            #+#    #+#             */
/*   Updated: 2024/08/14 13:07:44 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	**allocate_env_array(size_t count)
{
	return ((char **)malloc(sizeof(char *) * (count + 1)));
}

char	*create_env_entry(const char *name, const char *value)
{
	char	*tmp;
	char	*env_entry;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	env_entry = ft_strjoin(tmp, value);
	free(tmp);
	return (env_entry);
}

void	free_env_array(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

int	populate_env_array(char **env, t_envlst *env_lst)
{
	size_t	k;
	char	*env_entry;

	k = 0;
	while (env_lst != NULL)
	{
		if (env_lst->value != NULL)
		{
			env_entry = create_env_entry(env_lst->name, env_lst->value);
			if (!env_entry)
			{
				free_env_array(env);
				return (0);
			}
			env[k++] = env_entry;
		}
		env_lst = env_lst->next;
	}
	env[k] = NULL;
	return (1);
}

char	**own_env(t_envlst *env_lst)
{
	char	**env;
	size_t	env_count;

	env_count = count_list(env_lst);
	env = allocate_env_array(env_count);
	if (!env)
		return (NULL);
	if (!populate_env_array(env, env_lst))
	{
		free_env_array(env);
		return (NULL);
	}
	return (env);
}
