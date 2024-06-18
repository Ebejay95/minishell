/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:12 by chorst            #+#    #+#             */
/*   Updated: 2024/06/18 13:08:02 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Recreates the expport behavior from bash
void	ft_export(int argc, char **argv, char ****envp)
{
	int		i;
	char	**sorted_envp;

	i = 0;
	argv = NULL;
	sorted_envp = sort_envp(*envp);
	if (argc == 1)
		while (sorted_envp[i])
			ft_printf("declare -x \"%s\"\n", sorted_envp[i++]);
	// else
	// 	add_export(argv, &envp);
}

// sorts the envp array alphabetically
char	**sort_envp(char ***envp)
{
	int		i;
	int		j;
	char	*temp;
	char	**sorted_envp;

	sorted_envp = copy_envp(*envp);
	if (!sorted_envp)
		return (NULL);
	i = 0;
	while (sorted_envp[i + 1])
	{
		j = 0;
		while (sorted_envp[j + 1])
		{
			if (ft_strcmp(sorted_envp[j], sorted_envp[j + 1]) > 0)
			{
				temp = sorted_envp[j];
				sorted_envp[j] = sorted_envp[j + 1];
				sorted_envp[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	return (sorted_envp);
}

// copies the envp array to a new array
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
