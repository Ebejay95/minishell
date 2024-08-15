/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:00:01 by chorst            #+#    #+#             */
/*   Updated: 2024/08/15 19:38:48 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

void	init_env_list(char **envp, t_minishell *m)
{
	char	*name;
	char	*value;
	char	*new_shell_level;

	while (*envp != NULL)
	{
		name = NULL;
		value = NULL;
		extract_name_value(*envp, &name, &value);
		new_shell_level = NULL;
		if (ft_strcmp(name, "SHLVL") == 0)
		{
			new_shell_level = ft_itoa(ft_atoi(value) + 1);
			free(value);
		}
		else
			new_shell_level = value;
		if (ft_strcmp(name, "OLDPWD") != 0)
			add_env_node(&m->env_list, name, new_shell_level);
		else
			ft_multifree(name, new_shell_level, NULL, NULL);
		envp++;
	}
	add_env_node(&m->env_list, ft_strdup("OLDPWD"), NULL);
}

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
			*name = ft_strndup(arg, plus_sign - arg);
		else
			*name = ft_strndup(arg, equal_sign - arg);
		trimmed_value = ft_strtrim(equal_sign + 1, "\"");
		*value = trimmed_value;
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
	}
}

void	add_env_node(t_envlst **env_list, char *name, char *value)
{
	t_envlst	*new_node;
	t_envlst	*temp;

	new_node = malloc(sizeof(t_envlst));
	if (new_node == NULL)
	{
		free(name);
		free(value);
		return ((void)ft_printf(MEM_ERR));
	}
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
