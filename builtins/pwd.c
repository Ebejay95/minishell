/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:11:17 by chorst            #+#    #+#             */
/*   Updated: 2024/06/14 09:34:40 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	builtin_pwd(char **args)
{
	char	*pwd;

	(void)args;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return ;
	ft_printf("%s\n", pwd);
	free(pwd);
}
