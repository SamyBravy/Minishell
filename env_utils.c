/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:27:47 by marvin            #+#    #+#             */
/*   Updated: 2024/08/17 16:27:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *key, t_list *env)
{
	char	*identifier;
	char	*value;

	while (env)
	{
		identifier = get_key(env->content);
		if (!ft_strcmp(identifier, key))
		{
			value = ft_strchr(env->content, '=') + 1;
			free(identifier);
			return (value);
		}
		free(identifier);
		env = env->next;
	}
	return (NULL);
}

void	lst_remove_key(t_list **env, char *key)
{
	t_list	*tmp;
	char	*current_key;

	while (*env)
	{
		current_key = get_key((*env)->content);
		if (!ft_strcmp(current_key, key))
		{
			tmp = *env;
			*env = (*env)->next;
			free(tmp->content);
			free(tmp);
			free(current_key);
			break ;
		}
		free(current_key);
		env = &(*env)->next;
	}
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*get_key(char *str)
{
	char	*key;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}
