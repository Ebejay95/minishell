/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:42:12 by jkauker           #+#    #+#             */
/*   Updated: 2024/07/30 23:22:48 by jeberle          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/minishell.h"

// Counts the number of elements in the envp list
int	count_env_list(t_envlst *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		envp = envp->next;
		count++;
	}
	return (count);
}

// Prints the env variable in the format "declare -x NAME="VALUE"
void	print_env_variable(const char *envar)
{
	const char	*equal_sign;
	size_t		namelen;

	equal_sign = ft_strchr(envar, '=');
	if (equal_sign != NULL)
	{
		namelen = equal_sign - envar;
		printf("declare -x %.*s=\"%s\"\n", (int)namelen, envar, equal_sign + 1);
	}
	else
		printf("declare -x %s\n", envar);
}

// Funktion returned 0 wenn argv[0] mir einem Variablennamen überein stimmt
int	is_var_name(t_envlst *envp, char **argv)
{
	t_envlst	*current;

	current = envp;
	while (current != NULL)
	{
		if (!(ft_strncmp(current->name, argv[0], ft_strlen(current->name))))
			return (0);
		current = current->next;
	}
	return (1);
}

// Behaves like the strndup function from the standard library
char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	size_t	len;
	char	*result;

	i = 0;
	len = 0;
	while (len < n && s[len] != '\0')
		len++;
	result = (char *)malloc(len + 1);
	if (result == NULL)
		return (NULL);
	while (i < len)
	{
		result[i] = s[i];
		i++;
	}
	result[len] = '\0';
	return (result);
}
