/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonathaneberle <jonathaneberle@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:00:01 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 15:22:53 by jonathanebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that initializes the environment list
void	init_env_list(char **envp, t_minishell *m)
{
	char	*name;
	char	*value;
	char	*new_shell_level;

	while (*envp != NULL)
	{
		extract_name_value(*envp, &name, &value);
		new_shell_level = NULL;
		if (ft_strcmp(name, "SHLVL") == 0)
			new_shell_level = ft_itoa(ft_atoi(value) + 1);
		else
		{
			new_shell_level = value;
			value = NULL;
		}
		if (ft_strcmp(name, "OLDPWD") != 0)
			add_env_node(&m->env_list, name, new_shell_level);
		else
			free(name);
		envp++;
	}
	add_env_node(&m->env_list, "OLDPWD", NULL);
}

// Function that extracts the name and value of an environment variable
void	extract_name_value(char *arg, char **name, char **value)
{
	char	*equal_sign;
	char	*plus_sign;
	char	*trimmed_value;

	plus_sign = ft_strchr(arg, '+');
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		if (plus_sign && plus_sign < equal_sign)
			*name = ft_strndup(arg, plus_sign - arg); // allokierung
		else
			*name = ft_strndup(arg, equal_sign - arg);
		trimmed_value = ft_strtrim(equal_sign + 1, "\""); // allokierung
		*value = ft_strdup(trimmed_value); // allokierung
		free(trimmed_value);
	}
	else
	{
		*name = ft_strdup(arg); // allokierung
		*value = NULL;
	}
}

// Function that adds a node to the environment list
void	add_env_node(t_envlst **env_list, char *name, char *value)
{
	t_envlst	*new_node;
	t_envlst	*temp;

	new_node = malloc(sizeof(t_envlst));
	if (new_node == NULL)
		return ((void)ft_printf(MEM_ERR));
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (*env_list == NULL)
		*env_list = new_node;
	else
	{
		temp = *env_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
}

// Function that finds an environment variable in the list
t_envlst	*find_env_var(t_envlst *head, const char *name)
{
	t_envlst	*current;

	current = head;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
