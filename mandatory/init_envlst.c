/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkauker <jkauker@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:00:01 by chorst            #+#    #+#             */
/*   Updated: 2024/06/24 13:50:58 by jkauker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that initializes the environment list
t_envlst	*init_env_list(char **envp)
{
	t_envlst	*env_list;
	char		*name;
	char		*value;

	env_list = NULL;
	while (*envp != NULL)
	{
		extract_name_value(*envp, &name, &value);
		add_env_node(&env_list, name, value);
		envp++;
	}
	return (env_list);
}

// Function that extracts the name and value of an environment variable
void	extract_name_value(char *env_var, char **name, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign != NULL && *value != NULL)
	{
		*name = strndup(env_var, equal_sign - env_var);
		*value = strdup(equal_sign + 1);
	}
	else if (equal_sign != NULL && *value == NULL)
	{
		*name = strdup(env_var);
		*value = "";
	}
	else if (equal_sign == NULL && *name != NULL)
	{
		*name = strdup(env_var);
		*value = NULL;
	}
}

// Function that adds a node to the environment list
void	add_env_node(t_envlst **env_list, char *name, char *value)
{
	t_envlst	*new_node;
	t_envlst	*temp;

	new_node = malloc(sizeof(t_envlst));
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
		if (strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
