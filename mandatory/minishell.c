/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/06/18 13:49:37 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that parses the prompt into a table
void	parse_table(t_minishell *minishell)
{
	size_t	i;

	i = 0;
	while (minishell->prompt[i] != '\0')
	{
		minishell->parser.parse_pos = i;
		minishell->parser.parse_char = minishell->prompt[i];
		ft_printf("%i %c\n", i, minishell->prompt[i]);
		i++;
	}
}

// Function that prints the lexer structure
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
	// minishell->lexer.dquote_buffer = get_oc_segments(minishell->prompt,"\"");
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

// Returns a cleaned prompt without leading or trailing delimiters
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
		else if (i > 0 && !(clean_prompt[j - 1] == 32 || (clean_prompt[j
					- 1] >= 9 && clean_prompt[j - 1] <= 13)))
			clean_prompt[j++] = ' ';
		i++;
	}
	clean_prompt[j] = '\0';
	free(temp_prompt);
	return (clean_prompt);
}

// Function that parses the input into a table using ft_split ### TEMPORARY EXAMPLE ###
char	**parse_input(char *prompt, int *argc)
{
	char	**argv;
	int		i;
	char	*clean_prompt;

	clean_prompt = input_cleaner(prompt);
	argv = ft_split(clean_prompt, ' ');
	free(clean_prompt);
	i = 0;
	while (argv[i])
		i++;
	*argc = i;
	return (argv);
}

// Function that choses which bultin to execute based on the prompt
void	execute_command(char *prompt, char ***envp)
{
	int		argc;
	char	**argv;

	argv = parse_input(prompt, &argc);
	// if (ft_strcmp(prompt, "cd") == 0)
	// 	ft_cd(*envp, argv[1]);
	// if (ft_strcmp(prompt, "echo") == 0)
	// 	ft_echo(argv);
	if (ft_strcmp(prompt, "env") == 0)
		ft_env(envp);
	if (ft_strcmp(prompt, "exit") == 0)
		exit(0);
	if (ft_strcmp(prompt, "export") == 0)
		ft_export(argc, argv, &envp);
	if (ft_strcmp(prompt, "pwd") == 0)
		ft_pwd(argv);
	// if (ft_strcmp(prompt, "unset") == 0)		unset
	// 	ft_unset(envp, argv[1]);
	// lex_prompt(prompt);
	if (ft_strcmp(argv[0], "clear") == 0) // Handle clear as an external command
	{
		char *cmd = "/usr/bin/clear"; // Path to clear command
		argv[0] = cmd;
		if (execvp(cmd, argv) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
}

// Main function that runs the minishell loop
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
			execute_command(minishell.prompt, &minishell.envp);
			add_history(minishell.prompt);
			// lex_prompt(&minishell);
			free(minishell.prompt);
		}
	}
	return (0);
}
