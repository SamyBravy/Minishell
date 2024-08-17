/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 17:35:05 by marvin            #+#    #+#             */
/*   Updated: 2024/08/14 17:35:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_builtin(t_list *env)
{
	while (env)
	{
		if (ft_strchr(env->content, '='))
			ft_printf("%s\n", (char *)env->content);
		env = env->next;
	}
	return (0);
}
