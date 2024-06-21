/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:12 by chorst            #+#    #+#             */
/*   Updated: 2024/06/21 14:00:16 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"
// TODO += handlen ......!!!!!!!!!!!!!
// Recreates the expport behavior from bash
void	ft_export(t_envlst ***envp, int argc, char **argv)
{
	char	**envp_export;
	int		i;

	i = 0;
	if (argc == 1)
	{
		envp_export = copy_envp(**envp);
		sort_envp(envp_export);
		while (envp_export[i])
		{
			print_env_variable(envp_export[i]);
			free(envp_export[i]);
			i++;
		}
	}
	else
		export(&(**envp), argv);
}

// Adds or updates the env variables in the envp list
void	export(t_envlst **envp, char **argv)
{
	int		i;
	char	*name;
	char	*value;

	i = 1;
	while (argv[i])
	{
		extract_name_value(argv[i], &name, &value);
		if (name)
		{
			if (value)
				add_env_node(envp, name, value);
			else
				add_env_node(envp, name, "");
		}
		i++;
	}
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
	count = count_env_list(envp);
	envp_copy = malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
		return (NULL);
	while (envp)
	{
		name_equals = ft_strjoin(envp->name, "=");
		envp_copy[i] = ft_strjoin(name_equals, envp->value);
		free(name_equals);
		envp = envp->next;
		i++;
	}
	envp_copy[i] = NULL;
	return (envp_copy);
}

// Counts the number of elements in the envp list
int	count_env_list(t_envlst *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		envp = envp->next;
		count++;
	}
	return (count);
}

// Prints the env variable in the format "declare -x NAME="VALUE"
void	print_env_variable(const char *env_var)
{
	const char	*equal_sign;
	size_t		name_len;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign != NULL)
	{
		name_len = equal_sign - env_var;
		printf("declare -x %.*s=\"%s\"\n", (int)name_len, env_var, equal_sign + 1);
	}
	else
		printf("declare -x %s\n", env_var);
}
