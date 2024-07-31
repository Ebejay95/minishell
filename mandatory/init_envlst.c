/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:00:01 by chorst            #+#    #+#             */
/*   Updated: 2024/07/31 15:31:14 by chorst           ###   ########.fr       */
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
		if (ft_strcmp(name, "SHLVL") == 0)
			value = ft_itoa(atoi(value) + 1);
		if (ft_strcmp(name, "OLDPWD") != 0)
			add_env_node(&env_list, name, value);
		else
		{
			free(name);
			free(value);
		}
		envp++;
	}
	add_env_node(&env_list, "OLDPWD", NULL);
	return (env_list);
}

// Function that extracts the name and value of an environment variable
void	extract_name_value(char *arg, char **name, char **value)
{
	char	*equal_sign;
	char	*plus_sign;

	plus_sign = ft_strchr(arg, '+');
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		if (plus_sign && plus_sign < equal_sign)
			*name = ft_strndup(arg, plus_sign - arg);
		else
			*name = ft_strndup(arg, equal_sign - arg);
		*value = equal_sign + 1;
		*value = ft_strtrim(*value, "\"");
	}
	else
	{
		*name = ft_strdup(arg);
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
		return ;
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
