/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:27:04 by chorst            #+#    #+#             */
/*   Updated: 2024/08/07 14:17:57 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

char	*get_executable(t_minishell *m, char *command)
{
	char	*pathline;
	char	*joined;
	char	**paths;
	int		pathcount;
	int		i;

	pathline = my_getenv("PATH", m->env_list);
	if (pathline != NULL)
	{
		pathcount = ft_count_words(pathline, ':');
		paths = ft_split(pathline, ':');
		i = 0;
		while (i < pathcount)
		{
			joined = ft_strjoin(ft_strjoin(paths[i++], "/"), command);
			if (joined == NULL)
				break ;
			if (access(joined, X_OK) == 0)
				return (ft_array_l_free(paths, pathcount), joined);
			free(joined);
		}
		ft_array_l_free(paths, pathcount);
	}
	return (NULL);
}

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
