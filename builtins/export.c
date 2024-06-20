/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:12 by chorst            #+#    #+#             */
/*   Updated: 2024/06/19 17:40:14 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"
// TODO += handlen ......!!!!!!!!!!!!!
// Recreates the expport behavior from bash
void	ft_export(int argc, char **argv, char ****envp)
{
	int		i;
	char	**envp_export;

	i = 0;
	argv = NULL;
	envp_export = sort_envp(*envp);
	if (argc == 1)
		while (envp_export[i])
			ft_printf("declare -x \"%s\"\n", envp_export[i++]);
	// else
	// 	add_export(argv, &envp);// TODO += handlen ......!!!!!!!!!!!!!
}

// After copying envp to a envp_export, sorts the envp_export array
char	**sort_envp(char ***envp)
{
	int		i;
	int		j;
	char	*temp;
	char	**envp_export;

	envp_export = copy_envp(*envp);
	if (!envp_export)
		return (NULL);
	i = 0;
	while (envp_export[i + 1])
	{
		j = 0;
		while (envp_export[j + 1])
		{
			if (ft_strcmp(envp_export[j], envp_export[j + 1]) > 0)
			{
				temp = envp_export[j];
				envp_export[j] = envp_export[j + 1];
				envp_export[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	return (envp_export);
}

// copies the envp array to a new array to avoid modifying the original
char	**copy_envp(char **envp)
{
	int		i;
	char	**copied_envp;

	i = 0;
	while (envp[i])
		i++;
	copied_envp = malloc(sizeof(char *) * (i + 1));
	if (!copied_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copied_envp[i] = ft_strdup(envp[i]);
		if (!copied_envp[i])
		{
			free_it(copied_envp);
			return (NULL);
		}
		i++;
	}
	copied_envp[i] = NULL;
	return (copied_envp);
}

// void	add_export(char **argv, char *****envp)
// {
// }

void	free_it(char **str)
{
	char	**temp;

	temp = str;
	while (*temp)
		free(*temp++);
	free(str);
}
