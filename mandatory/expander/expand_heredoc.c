/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:27:57 by chorst            #+#    #+#             */
/*   Updated: 2024/08/13 18:08:28 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/minishell.h"

static int	append_str(char **dst, const char *src)
{
	char	*temp;
	size_t	dst_len;
	size_t	src_len;

	if (!dst || !*dst || !src)
		return (0);

	dst_len = ft_strlen(*dst);
	src_len = ft_strlen(src);

	temp = ft_calloc(dst_len + src_len + 1, sizeof(char));
	if (!temp)
		return (0);

	ft_strlcpy(temp, *dst, dst_len + 1);
	ft_strlcat(temp, src, dst_len + src_len + 1);

	free(*dst);
	*dst = temp;

	return (1);
}

static int	append_char(char **dst, char c)
{
	char	*temp;
	size_t	len;

	if (!dst || !*dst)
		return (0);

	len = ft_strlen(*dst);

	temp = ft_calloc(len + 2, sizeof(char));
	if (!temp)
		return (0);

	ft_strlcpy(temp, *dst, len + 1);
	temp[len] = c;

	free(*dst);
	*dst = temp;

	return (1);
}

static char	*expand_exit_status(t_minishell *m, char **result)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(m->last_exitcode);
	if (!exit_status_str || !append_str(result, exit_status_str))
	{
		free(exit_status_str);
		free(*result);
		return (NULL);
	}
	free(exit_status_str);
	return (*result);
}

char	*expand_var(t_minishell *m, char *str, size_t *i, char **result)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(str, i);
	if (var_name)
	{
		var_value = my_getenv(var_name, m->env_list);
		if (var_value && !append_str(result, var_value))
		{
			free(var_name);
			free(*result);
			return (NULL);
		}
		free(var_name);
	}
	else if (!append_char(result, '$'))
	{
		free(*result);
		return (NULL);
	}
	return (*result);
}

char	*expand_hd(t_minishell *m, char *str)
{
	char	*result;
	size_t	i;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			if (!expand_exit_status(m, &result))
				return (NULL);
			i++;
		}
		else if (str[i] == '$' && !expand_var(m, str, &i, &result))
			return (NULL);
		else if (str[i] != '"' && !append_char(&result, str[i]))
			return (NULL);
		i++;
	}
	return (result);
}
