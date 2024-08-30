/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:44:20 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:44:20 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_builtin(t_list *env)
{
	while (env)
	{
		if (((char *)env->content)[0] == '?')
		{
			env = env->next;
			continue ;
		}
		if (ft_strchr(env->content, '='))
			ft_printf("%s\n", (char *)env->content);
		env = env->next;
	}
	return (0);
}
