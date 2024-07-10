/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:10:58 by chorst            #+#    #+#             */
/*   Updated: 2024/07/10 16:07:20 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// #############################################################################
// #                          Helpfull Functiosn                               #
// #############################################################################
// 1. getcwd() gibt das aktuelle Arbeitsverzeichnis des aufrufenden Prozesses zurück
// 2. chdir() ändert das aktuelle Arbeitsverzeichnis des aufrufenden Prozesses
// 3. access() prüft, ob der Zugriff auf eine Datei möglich ist
// 4. getenv() gibt den Wert einer Umgebungsvariable zurück
// 5. stat() gibt Informationen über eine Datei zurück (lstat() und fstat() sind ähnlich)

// 6. strerror() gibt eine Zeichenfolge zurück, die die Fehlermeldung für die Fehlernummer enthält
// 7. perror() gibt die Fehlermeldung für die letzte Systemaufruf zurück

// function that changes the current working directory
void	ft_cd(int argc, char **argv, t_envlst ***envp)
{
	char	*path;
	char	*oldpwd;
	char	*pwd;

	path = NULL;
	oldpwd = NULL;
	pwd = NULL;
	if (argc == 1 || ft_strcmp(argv[1], "~") == 0)
	{
		path = getenv("HOME");
		if (!path) {
			printf("cd: HOME not set\n");
			return;
		}
	}
	else
		path = argv[1];
	oldpwd = getcwd(NULL, 0);
	if (oldpwd == NULL)
	{
		perror("getcwd");
		return;
	}
	if (chdir(path) == -1)
	{
		perror("chdir");
		free(oldpwd);
		return;
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("getcwd");
		free(oldpwd);
		return;
	}
	change_env_node(&(**envp), "OLDPWD", oldpwd, 0);
	change_env_node(&(**envp), "PWD", pwd, 0);
}



// getcwd vor und nach chdir aufrufen und vergleichen ob sich das Verzeichnis geändert hat
