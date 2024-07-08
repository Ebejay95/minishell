/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:42:23 by jkauker           #+#    #+#             */
/*   Updated: 2024/07/03 08:24:54 by jkauker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that changes the value of a variable
void	change_var_value(t_envlst **envp, char **argv)
{
	int			i;
	t_envlst	*current;
	char		*temp;

	i = 0;
	if (ft_strcmp(argv[0], "export") == 0)
		i++;
	while (argv[i])
	{
		current = *envp;
		while (ft_strncmp(current->name, argv[i], ft_strlen(current->name))
			&& current->next != NULL)
			current = current->next;
		temp = ft_strjoin(current->name, "+=");
		if (!(ft_strncmp(argv[i], temp, ft_strlen(temp))))
			upgrade_var_value(&(*envp), argv[i]);
		else
			my_export(&(*envp), &argv[i]);
		free(temp);
		i++;
	}
}

// Function that upgrades the value of a variable
void	upgrade_var_value(t_envlst **envp, char *str)
{
	t_envlst	*current;
	char		*name;
	char		*value;
	char		*temp;
	char		*temp2;

	extract_name_value(str, &name, &value);
	current = *envp;
	while (ft_strncmp(current->name, name, ft_strlen(current->name))
		&& current->next != NULL)
		current = current->next;
	temp = ft_strjoin(current->value, value);
	temp2 = ft_strtrim(temp, """");
	free(current->value);
	free(temp);
	current->value = temp2;
}

// Function that updates the value of a variable
void	update_var_value(t_envlst **envp, char *str)
{
	t_envlst	*current;
	char		*name;
	char		*value;

	extract_name_value(str, &name, &value);
	current = *envp;
	while (ft_strncmp(current->name, name, ft_strlen(current->name))
		&& current->next != NULL)
		current = current->next;
	free(current->value);
	current->value = value;
}
