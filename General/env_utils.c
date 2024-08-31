/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:43:16 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:43:16 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char *key, t_list *env)
{
	char	*current_key;
	char	*value;

	while (env)
	{
		current_key = get_key(env->content);
		if (!ft_strcmp(current_key, key))
		{
			if (ft_strchr(env->content, '=') == NULL)
				value = ft_strchr(env->content, '\0');
			else
				value = ft_strchr(env->content, '=') + 1;
			free(current_key);
			return (value);
		}
		free(current_key);
		env = env->next;
	}
	return (NULL);
}

int	ft_export(char *str, t_list **env, int check)
{
	char	*key;

	key = get_key(str);
	if (is_valid_identifier(key, check) && !(key[0] == '_' && key[1] == '\0'))
	{
		if (ft_strchr(str, '=') && ft_strchr(str, '+')
			&& ft_strchr(str, '=') == ft_strchr(str, '+') + 1)
		{
			export_append(str, key, env);
			return (free(key), 0);
		}
		if (ft_strchr(str, '=') != NULL)
			lst_remove_key(env, key);
		if (ft_getenv(key, *env) == NULL)
			ft_lstadd_back(env, ft_lstnew(ft_strdup(str)));
	}
	else if (!(key[0] == '_' && key[1] == '\0'))
	{
		ft_putstr_fd("minicecco: export: `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (free(key), 1);
	}
	free(key);
	return (0);
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

int	is_valid_identifier(char *str, int check)
{
	int	i;

	if (check)
		return (1);
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
	while (str[i] && str[i] != '='
		&& !(str[i] == '+' && str[i + 1] == '='))
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}
