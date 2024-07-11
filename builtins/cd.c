/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:10:58 by chorst            #+#    #+#             */
/*   Updated: 2024/07/11 12:58:51 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Main function that changes the current working directory
void	ft_cd(int argc, char **argv, t_envlst ***envp)
{
	char *path;
	char *oldpwd;

	if (argc == 1 || ft_strcmp(argv[1], "~") == 0)
		cd_home(envp);
	else
	{
		path = argv[1];
		oldpwd = getcwd(NULL, 0);
		if (!oldpwd)
			return (perror("getcwd"));
		if (chdir(path) == -1)
		{
			printf("🚀: cd: %s: No such file or directory\n", path);
			free(oldpwd);
			return;
		}
		char *pwd = getcwd(NULL, 0);
		if (!pwd)
			return (perror("getcwd"), free(oldpwd));
		change_env_node(&(**envp), "OLDPWD", oldpwd, 0);
		change_env_node(&(**envp), "PWD", pwd, 0);
		free(oldpwd);
	}
}

// helper function to change the current working directory to the home directory
void	cd_home(t_envlst ***envp)
{
	char *home = getenv("HOME");
	if (!home)
		return ((void)printf("cd: HOME not set\n"));
	char *oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("getcwd"));
	if (chdir(home) == -1)
	{
		printf("🚀: cd: %s: No such file or directory\n", home);
		free(oldpwd);
		return;
	}
	char *pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), free(oldpwd));
	change_env_node(&(**envp), "OLDPWD", oldpwd, 0);
	change_env_node(&(**envp), "PWD", pwd, 0);
	free(oldpwd);
}
