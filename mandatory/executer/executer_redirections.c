/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:29:28 by chorst            #+#    #+#             */
/*   Updated: 2024/08/08 14:31:50 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

int handle_redirections(t_minishell *m, t_list *ref, t_list *seq)
{
	int		fd;
	t_token	*token;
	char	*file_name;
	int		pipe_fd[2];

	fd = -1;
	file_name = NULL;
	while (seq != NULL) {
		token = (t_token *)seq->content;
		red_need_next_file(m, seq, token);
		if (token->token == REDIRECTION)
		{
			if (ft_strcmp(token->detail.rdrc.rdrctype, "truncate") == 0 || ft_strcmp(token->detail.rdrc.rdrctype, "append") == 0)
				{
				if (seq->next == NULL)
					break ;
				seq = seq->next;
				if (seq != NULL)
				{
					token = (t_token *)seq->content;
					if (token->token == WORD)
					{
						if (file_name != NULL)
						{
							free(file_name);
						}
						file_name = ft_strdup(token->str);
					}
				}
				if (seq->next == NULL)
					break ;
				seq = seq->next;
				while (seq != NULL)
				{
					token = (t_token *)seq->content;
					pic_err(m, 2, ft_strjoin(ft_strjoin(ft_strjoin(get_last_cmd(ref, seq), ": "), token->str), ": No such file or directory"));
					if (token->token == WORD)
					{
						if (file_name != NULL)
						{
							free(file_name);
						}
						file_name = strdup(token->str);
					}
					else
					{
						break ;
					}
					seq = seq->next;
				}
				if (file_name == NULL)
				{
					fprintf(stderr, "No file name specified for redirection\n");
					return -1;
				}
				if (ft_strcmp(token->detail.rdrc.rdrctype, "truncate"))
					fd = open(file_name, (O_WRONLY | O_CREAT | O_TRUNC), 0644);
				else
					fd = open(file_name, (O_WRONLY | O_CREAT | O_APPEND), 0644);
				if (fd == -1)
				{
					perror("Failed to open file for writing (truncate/append)");
					free(file_name);
					return -1;
				}
				if (dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("dup2 failed for stdout redirection");
					close(fd);
					free(file_name);
					return -1;
				}
				close(fd);

				free(file_name);
				file_name = NULL;
			}
			else if (ft_strcmp(token->detail.rdrc.rdrctype, "redirection") == 0)
			{
				seq = seq->next;
				if (seq != NULL) {
					token = (t_token *)seq->content;
					if (token->token == WORD)
						file_name = token->str;
					else
					{
						fprintf(stderr, "Expected a file name after redirection symbol\n");
						return -1;
					}
					fd = open(file_name, O_RDONLY);
					if (fd == -1)
					{
						perror("Failed to open file for reading");
						return -1;
					}
					if (dup2(fd, STDIN_FILENO) == -1)
					{
						perror("dup2 failed for stdin redirection");
						close(fd);
						return -1;
					}
					close(fd);
				}
			} else if (ft_strcmp(token->detail.rdrc.rdrctype, "here_doc") == 0)
			{
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe failed for heredoc");
					return (-1);
				}
				write(pipe_fd[1], token->str, strlen(token->str));
				close(pipe_fd[1]);
				if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
				{
					perror("dup2 failed for heredoc stdin");
					close(pipe_fd[0]);
					return (-1);
				}
				close(pipe_fd[0]);
			}
		}
		else
		{
			seq = seq->next;
		}
	}
	return (0);
}
