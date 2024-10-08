/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:12 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 15:23:31 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

// Recreates the expport behavior from bash
void	ft_export(int argc, char **argv, t_envlst **envp)
{
	int		i;
	char	**envp_export;

	i = 0;
	if (argc == 1 && ft_strcmp(argv[0], "export") == 0)
	{
		envp_export = copy_envp(*envp);
		sort_envp(envp_export);
		while (envp_export[i])
		{
			print_env_variable(envp_export[i]);
			free(envp_export[i++]);
		}
		free(envp_export);
	}
	else
		change_var_value(&(*envp), argv);
}

// Adds or updates the env variables in the envp list
void	my_export(t_envlst **envp, char **argv)
{
	int		i;
	char	*name;
	char	*value;

	i = 0;
	while (argv[i])
	{
		extract_name_value(argv[i], &name, &value);
		if (name)
			change_env(envp, name, value, 1);
		i++;
	}
}

// Adds or updates a node in the env list with the given name and value
// or deletes it if value is NULL and free_it is 1
void	change_env(t_envlst **env_lst, char *key, char *value, int free_it)
{
	t_envlst	*current;

	current = *env_lst;
	while (current)
	{
		if (ft_strcmp(current->name, key) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			if (free_it)
			{
				free(key);
				free(value);
			}
			return ;
		}
		current = current->next;
	}
	add_env_node(env_lst, key, value);
}

// Sorts a char ** array of env variables alphabetically
void	sort_envp(char **envp)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
}

// copies the envp array to a new array to avoid modifying the original
char	**copy_envp(t_envlst *envp)
{
	int		i;
	int		count;
	char	**envp_copy;
	char	*name_equals;

	i = 0;
	count = count_list(envp);
	envp_copy = malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
		return (NULL);
	while (envp)
	{
		if (envp->value)
		{
			name_equals = ft_strjoin(envp->name, "=");
			envp_copy[i] = ft_strjoin(name_equals, envp->value);
			free(name_equals);
		}
		else
			envp_copy[i] = ft_strdup(envp->name);
		envp = envp->next;
		i++;
	}
	envp_copy[i] = NULL;
	return (envp_copy);
}
