/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/01 11:37:47 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	check_semantics(t_list *last, t_list *current)
{
	t_token	*last_token;
	t_token	*token;

	last_token = NULL;
	token = (t_token *)current->content;
	if (last)
	{
		last_token = (t_token *)last->content;
		if (ft_strcmp(token->type, "Word") == 0 && ft_strcmp(last_token->type, "Redirection") == 0 && ft_strcmp(last_token->str, "<<") == 0)
		{
			update_tok_type(token, DELIMITER);
		}
		if (ft_strcmp(token->type, "Word") == 0 && ft_strcmp(last_token->type, "Pipe") == 0 && ft_strcmp(last_token->str, "|") == 0)
		{
			update_tok_type(token, COMMAND);
		}
	}
	else
	{
		if (ft_strcmp(token->type, "Word") == 0)
		{
			update_tok_type(token, COMMAND);
		}
	}
}

t_token	*init_pipe_details(t_token *pipetok)
{
	pipetok->detail.pipe.fdin = -1;
	pipetok->detail.pipe.fdout = -1;
	pipetok->detail.pipe.open_prompt = 0;
	return (pipetok);
}

t_token	*init_redirection_details(t_token *redirectiontoken)
{
	redirectiontoken->detail.redirection.fdin = -1;
	redirectiontoken->detail.redirection.fdout = -1;
	redirectiontoken->detail.redirection.rdrctype = NULL;
	return (redirectiontoken);
}

void	pre_exec_prep(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;

	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0)
			cur_content = init_pipe_details(cur_content);
		else if (ft_strcmp(cur_content->type, "Redirection") == 0)
			init_redirection_details(cur_content);
		current = current->next;
	}
}

int	check_pipes(t_minishell *m)
{
	t_list	*current;
	t_token	*cur_content;
	char	*last_type;
	char	*last_str;

	last_type = NULL;
	last_str = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0 && last_type == NULL)
		{
			ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
			return 2;
		}
		else if (current->next == NULL && ft_strcmp(cur_content->type, "Pipe") == 0)
		{
			cur_content->detail.pipe.open_prompt = 1;
		}
		last_type = cur_content->type;
		last_str = cur_content->str;
		current = current->next;
	}
	return 0;
}

void	set_rdrctype(t_list *last, t_list *current, t_token *token)
{
	check_semantics(last, current);
	if (ft_strcmp(token->type, "Redirection") == 0)
	{
		if (ft_strcmp(token->str, "<<") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("here_doc");
		}
		if (ft_strcmp(token->str, ">>") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("append");
		}
		if (ft_strcmp(token->str, "<") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("redirection");
		}
		if (ft_strcmp(token->str, ">") == 0)
		{
			token->detail.redirection.rdrctype = ft_strdup("truncate");
		}
	}
}

int	check_redirections(t_minishell *m)
{
	t_list	*current;
	t_list	*last;
	t_token	*cur_content;
	char	*last_type;
	char	*last_str;

	last = NULL;
	last_type = NULL;
	last_str = NULL;
	current = m->tok_lst;
	while (current != NULL)
	{
		cur_content = (t_token *)current->content;
		if (ft_strcmp(cur_content->type, "Pipe") == 0 && ft_strcmp(last_type, "Redirection") == 0)
		{
			if (ft_strcmp(last_str, ">") != 0)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
				return 2;
			}
		}
		else if (current->next == NULL && ft_strcmp(cur_content->type, "Redirection") == 0)
		{
			ft_fprintf(2, "bash: syntax error near unexpected token `newline'\n");
			return 2;
		}
		else if (ft_strcmp(last_type, "Redirection") == 0 && ft_strcmp(cur_content->type, "Redirection") == 0)
		{
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, ">") == 0)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `>'\n");
				return 2;
			}
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, ">>") == 0)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `>>'\n");
				return 2;
			}
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, "<") == 0)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `<'\n");
				return 2;
			}
			if ((ft_strcmp(last_str, "<<") == 0 || ft_strcmp(last_str, ">>") == 0) && ft_strcmp(cur_content->str, "<<") == 0)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `<<'\n");
				return 2;
			}
		}
		set_rdrctype(last, current, cur_content);
		last_type = cur_content->type;
		last_str = cur_content->str;
		last = current;
		current = current->next;
	}
	return 0;
}

int	pre_exec_checks(t_minishell *m)
{
	if (m->exitcode == 0)
		m->exitcode = check_pipes(m);
	if (m->exitcode == 0)
		m->exitcode = check_redirections(m);
	return m->exitcode;
}

void execute(t_minishell *m)
{
	int		pipe_fd[2];
	int		pid;
	int		prev_pipe_read;
	t_list	*current;
	t_token	*token;
	int		status;

	pre_exec_prep(m);
	if (m->exitcode == 0)
		m->exitcode = pre_exec_checks(m);
	if (m->exitcode == 0)
	{
		ft_printf(Y"EXECUTE:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
	if (m->exitcode != 0)
		return;
	prev_pipe_read = STDIN_FILENO;
	current = m->tok_lst;
	while (current != NULL)
	{
		token = (t_token *)current->content;
		if (token->token == PIPE)
		{
			if (pipe(pipe_fd) == -1)
				handle_error(m, "Pipe creation failed");
			pid = fork();
			if (pid == -1)
				handle_error(m, "Fork failed");
			else if (pid == 0)
			{
				close(pipe_fd[0]);
				dup2(prev_pipe_read, STDIN_FILENO);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				run_command(m, current);
				exit(EXIT_SUCCESS);
			}
			else
			{
				close(pipe_fd[1]);
				if (prev_pipe_read != STDIN_FILENO)
					close(prev_pipe_read);
				prev_pipe_read = pipe_fd[0];
			}
		}
		else if (token->token == COMMAND)
		{
			pid = fork();
			if (pid == -1)
				handle_error(m, "Fork failed");
			else if (pid == 0)
			{
				if (prev_pipe_read != STDIN_FILENO)
				{
					dup2(prev_pipe_read, STDIN_FILENO);
					close(prev_pipe_read);
				}
				run_command(m, current);
				exit(EXIT_SUCCESS);
			}
		}
		current = current->next;
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			m->exitcode = WEXITSTATUS(status);
	}
	if (prev_pipe_read != STDIN_FILENO)
		close(prev_pipe_read);
}
