/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 17:01:26 by jeberle           #+#    #+#             */
/*   Updated: 2024/07/31 22:10:45 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	check_semantics(t_list *last, t_list *current, t_token *token, t_list *current->next)
{
	ft_printf("check semantics\n");
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
		//check_semantics(NULL, current, token, current->next);
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
	check_semantics(last, current, token, current->next);
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
		set_rdrctype(last, current, cur_content);
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

//void	exec_interpreter()

void	execute(t_minishell *m)
{
	pre_exec_prep(m);
	if (m->exitcode == 0)
		m->exitcode = pre_exec_checks(m);
	if (m->exitcode == 0)
	{
		//exec_interpreter(m);
		ft_printf(Y"EXECUTE:\n"D);
		ft_lstput(&(m->tok_lst), put_token, '\n');
	}
}
