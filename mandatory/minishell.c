/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/14 12:32:05 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	ft_putallenv(char **env)
{
	int		i;
	char	*print;

	i = 0;
	while (env[i] != 0)
	{
		print = ft_color(env[i], YELLOW);
		ft_printf(print);
		ft_printf("\n");
		free(print);
		i++;
	}
}

void	parse_table(t_minishell *minishell)
{
	size_t		i;

	i = 0;
	while (minishell->prompt[i] != '\0')
	{
		minishell->parser.parse_pos = i;
		minishell->parser.parse_char = minishell->prompt[i];
		ft_printf("%i %c\n", i, minishell->prompt[i]);
		i++;
	}
}

void	lex_prompt(t_minishell *minishell)
{
	ft_printf("\x1b[36m%s\x1b[0m\n", minishell->prompt);
	minishell->lexer.is_unclosed_quote = 1;
	minishell->lexer.position = 0;
	minishell->lexer.is_lessthan = 0;
	minishell->lexer.is_greaterthan = 0;
	minishell->lexer.is_space = 0;
	minishell->lexer.is_dash = 0;
	minishell->lexer.is_bslash = 0;
	minishell->lexer.is_dollar = 0;
	minishell->lexer.is_questmark = 0;
	minishell->lexer.is_ampersand = 0;
	minishell->lexer.is_semicolon = 0;
	minishell->lexer.is_pipe = 0;
	minishell->lexer.is_logical_and = 0;
	minishell->lexer.is_logical_or = 0;
	minishell->lexer.op_quote = 0;
	minishell->lexer.op_dquote = 0;
	minishell->lexer.cl_quote = 0;
	minishell->lexer.cl_dquote = 0;
	minishell->lexer.pipe_buffer = get_segments(minishell->prompt, "|");
	// minishell->lexer.quote_buffer = get_oc_segments(minishell->prompt, "'");
	// minishell->lexer.dquote_buffer = get_oc_segments(minishell->prompt, "\"");
	minishell->lexer.quote_buffer = get_quote_segments(minishell, '\'');
	minishell->lexer.dquote_buffer = get_quote_segments(minishell, '"');
	minishell->lexer.inrdrct_buffer = get_segments(minishell->prompt, "<");
	minishell->lexer.outrdrct_buffer = get_segments(minishell->prompt, ">");
	minishell->lexer.appoutrdrct_buffer = get_segments(minishell->prompt, ">>");
	minishell->lexer.heredoc_buffer = get_segments(minishell->prompt, "<<");
	minishell->lexer.space_buffer = get_segments(minishell->prompt, " ");
	minishell->lexer.option_buffer = get_segments(minishell->prompt, "-");
	minishell->lexer.ampersand_buffer = get_segments(minishell->prompt, "&");
	minishell->lexer.semicolon_buffer = get_segments(minishell->prompt, ";");
	minishell->lexer.variable_buffer = get_segments(minishell->prompt, "$");
	minishell->lexer.equal_buffer = get_segments(minishell->prompt, "=");
	put_lexer(minishell->lexer);
	parse_table(minishell);
}

char	*input_cleaner(char *prompt)
{
	int		i;
	int		j;
	char	*temp_prompt;
	char	*clean_prompt;

	i = 0;
	j = 0;
	temp_prompt = ft_strtrim(prompt, " \t\n\v\f\r");
	clean_prompt = (char *)malloc(strlen(temp_prompt) + 1);
	while (temp_prompt[i])
	{
		if (temp_prompt[i] != 32 && !(temp_prompt[i] >= 9
				&& temp_prompt[i] <= 13))
			clean_prompt[j++] = temp_prompt[i];
		else if (i > 0 && !(clean_prompt[j - 1] == 32 || (clean_prompt[j - 1] >= 9 && clean_prompt[j - 1] <= 13)))
			clean_prompt[j++] = ' ';
		i++;
	}
	clean_prompt[j] = '\0';
	free(temp_prompt);
	return (clean_prompt);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argv;
	(void)envp;
	if (argc != 1)
		return (0);
	while (1)
	{
		minishell.refenvp = envp;
		minishell.envp = envp;
		minishell.prompt = readline("\033[0;31m8==D \033[0m");
		minishell.prompt = input_cleaner(minishell.prompt);
		if (minishell.prompt)
		{
			if (strcmp(minishell.prompt, "pwd") == 0)
				builtin_pwd(argv);
			else if(strcmp(minishell.prompt, "cd") == 0)
				builtin_cd(argv);
			else if (strcmp(minishell.prompt, "echo") == 0)
				builtin_echo(argv);
			// else if (strcmp(minishell.prompt, "export") == 0)
			// 	builtin_export(argv);
			else if (strcmp(minishell.prompt, "unset") == 0)
				builtin_unset(argv);
			// else if (strcmp(minishell.envp, "env") == 0)
			// 	builtin_env(argv);
			else if (strcmp(minishell.prompt, "exit") == 0)
				builtin_exit(argv);
			else if (strcmp(minishell.prompt, "env") == 0)
				ft_putallenv(envp);
			else if (strcmp(minishell.prompt, "exit") == 0)
				exit(0);
			add_history(minishell.prompt);
			lex_prompt(&minishell);
			free(minishell.prompt);
		}
		else
			return (ft_putstr_fd(2, "cant read input\n"), 1);
	}
	return (0);
}
