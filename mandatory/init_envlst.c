/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:00:01 by chorst            #+#    #+#             */
/*   Updated: 2024/08/06 22:41:54 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Function that initializes the environment list
void	init_env_list(char **envp, t_minishell *m)
{
	char	*name;
	char	*value;
	int		shlvl;

	while (*envp != NULL)
	{
		extract_name_value(*envp, &name, &value);
		if (ft_strcmp(name, "SHLVL") == 0)
		{
			shlvl = ft_atoi(value);
			value = ft_itoa(shlvl + 1);
		}
		if (ft_strcmp(name, "OLDPWD") != 0)
			add_env_node(&m->env_list, name, value);
		else
		{
			free(name);
			free(value);
		}
		envp++;
	}
	add_env_node(&m->env_list, "OLDPWD", NULL);
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
	{
		ft_printf("Memory allocation failed\n");
		return ;
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (*env_list == NULL)
	{
		*env_list = new_node;
	}
	else
	{
		temp = *env_list;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
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
