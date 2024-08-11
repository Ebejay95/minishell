/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_funcitons.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:05:17 by chorst            #+#    #+#             */
/*   Updated: 2024/08/11 14:15:45 by jeberle          ###   ########.fr       */
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

// void	run_command(t_minishell *m, t_list *current)
// {
// 	t_token		*token;
// 	t_envlst	**envlist;
// 	t_list		*arg_list;
// 	int			i;
// 	int			argc;
// 	int			noword;
// 	char		**argv;
// 	char		*command;

// 	i = 0;
// 	argc = 0;
// 	noword = 0;
// 	if (current == NULL)
// 		return ;
// 	envlist = &(m->env_list);
// 	token = (t_token *)current->content;
// 	command = whitespace_handler(token->str);
// 	current = current->next;
// 	arg_list = current;
// 	noword = 0;
// 	while (arg_list != NULL && noword == 0)
// 	{
// 		token = (t_token *)arg_list->content;
// 		if (token->token == WORD)
// 			argc++;
// 		else
// 			noword = 1;
// 		arg_list = arg_list->next;
// 	}
// 	argv = (char **)malloc((argc + 2) * sizeof(char *));
// 	if (!argv)
// 		return (perror("malloc failed"));
// 	argv[i++] = ft_strdup(command);
// 	if (!argv[0])
// 		return (perror("ft_strdup failed"), free(argv));
// 	noword = 0;
// 	while (current != NULL && noword == 0)
// 	{
// 		token = (t_token *)current->content;
// 		if (token->token == WORD)
// 		{
// 			argv[i] = ft_strdup(token->str);
// 			if (!argv[i])
// 			{
// 				perror("ft_strdup failed");
// 				while (i > 0)
// 					free(argv[--i]);
// 				return (free(argv));
// 			}
// 			i++;
// 		}
// 		else
// 			noword = 1;
// 		current = current->next;
// 	}
// 	argv[i] = NULL;
// 	if (ft_strcmp(command, "cd") == 0)
// 		ft_cd(i, argv, &envlist);
// 	else if (!ft_strcmp(command, "echo"))
// 		ft_echo(argv);
// 	else if (!ft_strcmp(command, "env"))
// 		ft_env(*envlist);
// 	else if (!ft_strcmp(command, "exit"))
// 		ft_exit(argv);
// 	else if (!ft_strcmp(command, "export"))
// 		ft_export(i, argv, &envlist);
// 	else if (!is_var_name(*envlist, &argv[0]))
// 		ft_export(i, argv, &envlist);
// 	else if (!ft_strcmp(command, "pwd"))
// 		ft_pwd(argv);
// 	else if (!ft_strcmp(command, "unset"))
// 		ft_unset(envlist, argv);
// 	else
// 		ft_run_others(m, command, argv);
// 	i = 0;
// 	while (argv[i] != NULL)
// 		free(argv[i++]);
// 	if (DEBUG == 1)
// 		ft_printf(B"exitcode: %i\n"D, m->exitcode);
// 	free(argv);
// }

// int	handle_redirections(t_minishell *m, t_list *ref, t_list *seq)
// {
// 	int		fd;
// 	int		pipe_fd[2];
// 	char	*file_name;
// 	t_token	*token;

// 	fd = -1;
// 	file_name = NULL;
// 	while (seq != NULL)
// 	{
// 		token = (t_token *)seq->content;
// 		if (token->token == REDIRECTION)
// 		{
// 			if (!ft_strcmp(token->detail.redirection.rdrcmeta, "truncate")
// 				|| !ft_strcmp(token->detail.redirection.rdrcmeta, "append"))
// 			{
// 				if (seq->next == NULL)
// 					break ;
// 				seq = seq->next;
// 				if (seq != NULL)
// 				{
// 					token = (t_token *)seq->content;
// 					if (token->token == WORD)
// 					{
// 						if (file_name != NULL)
// 							free(file_name);
// 						file_name = ft_strdup(token->str);
// 					}
// 				}
// 				if (seq->next == NULL)
// 					break ;
// 				seq = seq->next;
// 				while (seq != NULL)
// 				{
// 					token = (t_token *)seq->content;
// 					pic_err(m, 2, ft_strjoin(ft_strjoin(ft_strjoin(get_last_cmd(ref, seq), ": "), token->str), ": No such file or directory"));
// 					if (token->token == WORD)
// 					{
// 						if (file_name != NULL)
// 							free(file_name);
// 						file_name = strdup(token->str);
// 					}
// 					else
// 						break ;
// 					seq = seq->next;
// 				}
// 				if (file_name == NULL)
// 				{
// 					fprintf(stderr, "No file name specified for redirection\n");
// 					return (-1);
// 				}
// 				if (ft_strcmp(token->detail.redirection.rdrcmeta, "truncate"))
// 					fd = open(file_name, (O_WRONLY | O_CREAT | O_TRUNC), 0644);
// 				else
// 					fd = open(file_name, (O_WRONLY | O_CREAT | O_APPEND), 0644);
// 				if (fd == -1)
// 				{
// 					perror("Failed to open file for writing (truncate/append)");
// 					free(file_name);
// 					return (-1);
// 				}
// 				if (dup2(fd, STDOUT_FILENO) == -1)
// 				{
// 					perror("dup2 failed for stdout redirection");
// 					close(fd);
// 					free(file_name);
// 					return (-1);
// 				}
// 				close(fd);
// 				free(file_name);
// 				file_name = NULL;
// 			}
// 			else if (!ft_strcmp(token->detail.redirection.rdrcmeta, "redirection"))
// 			{
// 				seq = seq->next;
// 				if (seq != NULL)
// 				{
// 					token = (t_token *)seq->content;
// 					if (token->token == WORD)
// 						file_name = token->str;
// 					else
// 					{
// 						fprintf(stderr, "Expected a file name after redirection symbol\n");
// 						return (-1);
// 					}
// 					fd = open(file_name, O_RDONLY);
// 					if (fd == -1)
// 					{
// 						perror("Failed to open file for reading");
// 						return (-1);
// 					}
// 					if (dup2(fd, STDIN_FILENO) == -1)
// 					{
// 						perror("dup2 failed for stdin redirection");
// 						close(fd);
// 						return (-1);
// 					}
// 					close(fd);
// 				}
// 			}
// 			else if (ft_strcmp(token->detail.redirection.rdrcmeta, "here_doc") == 0)
// 			{
// 				if (pipe(pipe_fd) == -1)
// 				{
// 					perror("pipe failed for heredoc");
// 					return (-1);
// 				}
// 				write(pipe_fd[1], token->str, strlen(token->str));
// 				close(pipe_fd[1]);
// 				if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
// 				{
// 					perror("dup2 failed for heredoc stdin");
// 					close(pipe_fd[0]);
// 					return (-1);
// 				}
// 				close(pipe_fd[0]);
// 			}
// 		}
// 		else
// 		{
// 			seq = seq->next;
// 		}
// 	}
// 	return (0);
// }
