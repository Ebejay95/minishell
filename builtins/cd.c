/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:10:58 by chorst            #+#    #+#             */
/*   Updated: 2024/06/13 13:23:33 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_cd(char **args)
{
	char	*home;
	char	*pwd;
	int		ret;

	home = ft_getenv("HOME");
	if (args[1] == NULL)
	{
		ret = chdir(home);
		// if (ret == -1)
		// 	ft_printf("cd: %s: %s\n", home, strerror(errno));
	}
	else if (args[2] != NULL)
		ft_printf("cd: too many arguments\n");
	else
	{
		ret = chdir(args[1]);
		// if (ret == -1)
		// 	ft_printf("cd: %s: %s\n", args[1], strerror(errno));
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return ;
	ft_setenv("PWD", pwd, 1);
	free(pwd);
}
