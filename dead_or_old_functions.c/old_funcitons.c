/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_execute_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:05:17 by chorst            #+#    #+#             */
/*   Updated: 2024/08/02 11:07:32 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// // Function that parses the input into a table using ft_split
// char	**parse_input(char *prompt, int *argc)
// {
// 	char	**argv;
// 	int		i;

// 	argv = ft_split(prompt, ' ');
// 	i = 0;
// 	while (argv[i])
// 		i++;
// 	*argc = i;
// 	return (argv);
// }

// // Function that choses which bultin to execute based on the prompt
// void	execute_command(t_minishell *minishell)
// {
// 	int			argc;
// 	char		**argv;
// 	t_envlst	**envlist;

// 	envlist = &(minishell->env_list);
// 	argv = parse_input(minishell->prompt, &argc);
// 	if (argv[0])
// 	{
// 		if (ft_strcmp(argv[0], "cd") == 0)
// 			ft_cd(argc, argv, &envlist);
// 		if (ft_strcmp(argv[0], "echo") == 0)
// 			ft_echo(argv);
// 		if (ft_strcmp(minishell->prompt, "env") == 0)
// 			ft_env(*envlist);
// 		if (!(ft_strcmp(argv[0], "exit")))
// 			ft_exit(argv);
// 		if (!(ft_strcmp(argv[0], "export")) || !(is_var_name(*envlist, argv)))
// 			ft_export(argc, argv, &envlist);
// 		if (ft_strcmp(minishell->prompt, "pwd") == 0)
// 			ft_pwd(argv);
// 		if (ft_strcmp(argv[0], "unset") == 0)
// 			ft_unset(envlist, argv);
// 	}
// }
