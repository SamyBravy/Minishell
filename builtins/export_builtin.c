/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:08:16 by marvin            #+#    #+#             */
/*   Updated: 2024/08/17 13:08:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sort_keys(t_list *env)
{
	t_list	*tmp;
	char	*tmp_content;
	char	*key1;
	char	*key2;

	while (env)
	{
		tmp = env->next;
		while (tmp)
		{
			key1 = get_key(env->content);
			key2 = get_key(tmp->content);
			if (ft_strcmp(key1, key2) > 0)
			{
				tmp_content = env->content;
				env->content = tmp->content;
				tmp->content = tmp_content;
			}
			free(key1);
			free(key2);
			tmp = tmp->next;
		}
		env = env->next;
	}
}

static void	print_export(t_list *env)
{
	int		i;

	sort_keys(env);
	while (env)
	{
		i = 0;
		ft_printf("declare -x ");
		while (((char *)env->content)[i] != '=' && ((char *)env->content)[i])
			ft_printf("%c", ((char *)env->content)[i++]);
		if (ft_strchr(env->content, '='))
			ft_printf("=\"%s\"", ft_strchr(env->content, '=') + 1);
		ft_printf("\n");
		env = env->next;
	}
}

void	export_append(char *str, char *key, t_list **env)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin(key, "=");
	if (ft_getenv(key, *env) == NULL)
	{
		tmp2 = ft_strjoin(tmp1, ft_strchr(str, '=') + 1);
		free(tmp1);
		ft_lstadd_back(env, ft_lstnew(tmp2));
		return ;
	}
	tmp2 = ft_strjoin(tmp1, ft_getenv(key, *env));
	free(tmp1);
	tmp1 = ft_strjoin(tmp2, ft_strchr(str, '=') + 1);
	free(tmp2);
	lst_remove_key(env, key);
	ft_lstadd_back(env, ft_lstnew(tmp1));
}

int	export_builtin(char **argv, t_list **env)
{
	int		i;
	int		exit_status;

	if (!argv[1])
		print_export(*env);
	exit_status = 0;
	i = 0;
	while (argv[++i])
		if (ft_export(argv[i], env) == 1)
			exit_status = 1;
	return (exit_status);
}
