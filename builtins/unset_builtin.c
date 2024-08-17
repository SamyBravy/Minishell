/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 19:12:08 by marvin            #+#    #+#             */
/*   Updated: 2024/08/16 19:12:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	lst_remove_key(t_list **env, char *key)
{
	t_list	*tmp;

	while (*env)
	{
		if (!ft_strncmp((*env)->content, key, ft_strlen(key))
			&& (((char *)(*env)->content)[ft_strlen(key)] == '='
				|| ((char *)(*env)->content)[ft_strlen(key)] == '\0'))
		{
			tmp = *env;
			*env = (*env)->next;
			free(tmp->content);
			free(tmp);
			break ;
		}
		env = &(*env)->next;
	}
}

int	unset_builtin(char **argv, t_list **env)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = 1;
		}
		else
			lst_remove_key(env, argv[i]);
		i++;
	}
	return (exit_status);
}
