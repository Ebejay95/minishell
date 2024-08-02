/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:01:33 by jeberle           #+#    #+#             */
/*   Updated: 2024/08/02 10:22:51 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that parses the input into a table using ft_split
// Diese Funktion ist nur ein temporaer und kann natuerlich komplett umgeschrieben werden, oder durch deine eigene Funktion ersetzt werden
char	**parse_input(char *prompt, int *argc)
{
	char	**argv;
	int		i;

	argv = ft_split(prompt, ' ');
	i = 0;
	while (argv[i])
		i++;
	*argc = i;
	return (argv);
}

// Function that choses which bultin to execute based on the prompt
// Dieser Funktion kannst du natuerlich noch weitere Befehle hinzufuegen, oder komplett umschreiben
void	execute_command(char *prompt, t_envlst **envlst)
{
	int		argc;
	char	**argv;

	argv = parse_input(prompt, &argc);
	if (argv[0])
	{
		if (ft_strcmp(argv[0], "cd") == 0)
			ft_cd(argc, argv, &envlst);
		if (ft_strcmp(argv[0], "echo") == 0)
			ft_echo(argv);
		if (ft_strcmp(prompt, "env") == 0)
			ft_env(*envlst);
		if (!(ft_strcmp(argv[0], "exit")))
			ft_exit(argv);
		if (!(ft_strcmp(argv[0], "export")) || !(is_var_name(*envlst, argv)))
			ft_export(argc, argv, &envlst);
		if (ft_strcmp(prompt, "pwd") == 0)
			ft_pwd(argv);
		if (ft_strcmp(argv[0], "unset") == 0)
			ft_unset(envlst, argv);
	}
	//while (argc >= 0)
	//	free(argv[argc--]);
}

// Function that handles the input from the user or the script
// minishell->interactive wird in der setup_signals Funktion initialisiert
// minishell->interactive 1 = interactive mode
// minishell->interactive 0 = non-interactive mode
// Je nach dem also ob unsere minishell im interaktiven oder nicht-interaktiven Modus laeuft, wird die entsprechende Funktion aufgerufen
int	handle_input(t_minishell *minishell, t_envlst **envlst)
{
	if (minishell->is_interactive)
		interactive_mode(&(*minishell));
	else
		non_interactive_mode(&(*minishell));
	if (!minishell->prompt)
		return (0);
	minishell->exitcode = 0;
	minishell->tok_lst = NULL;
	minishell->ast = ft_btreenew(NULL);
	if (minishell->prompt[0] != '\0')
	{
		execute_command(minishell->prompt, envlst);
		lex_prompt(minishell);
		//execute(minishell);
		// parse(minishell);
		if (minishell->is_interactive)
			add_history(minishell->prompt);
	}
	cleanup_minishell(minishell);
	return (1);
}

// Du kannst alle deutschen Kommentare loeschen. Die sind nur fuer dich, damit du weiss, was ich gemacht habe.
// Was vorher in der main war, steht jetzt in handle_input (einfach ausgelagert). Ich habe zusatzlich den interactive mode und non-interactive mode in handle_input eingefügt)
// #############################################################################

// Main function that runs the minishell loop
int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	t_envlst	*envlst;

	envlst = init_env_list(envp, &minishell);
	(void)argv;
	if (argc != 1)
		return (0);
	setup_signals(&minishell);
	initialize_minishell(&minishell, envp);
	while (1)
	{
		minishell.envp = envp;
		minishell.prompt = readline("🍕🚀🌈🦄🍺 ");
		if (!minishell.prompt)
		{
			printf("🍕🚀🌈🦄🍺 exit\n");
			exit(0);
		}
		minishell.tok_lst = NULL;
		minishell.ast = ft_btreenew(NULL);
		if (minishell.prompt)
		{
			execute_command(minishell.prompt, &envlst);
			add_history(minishell.prompt);
			lex_prompt(&minishell);
			execute(&minishell);
			// parse(&minishell);
			//free(minishell.prompt);
		}
	}
	//	if (!handle_input(&minishell, &envlst))
	//		break ;
	rl_clear_history();
	return (0);
}
