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

void	print_export(t_list *env)
{
	t_list	*sorted_env;
	int		i;

	sorted_env = ft_lstclone(env);
	sort_keys(sorted_env);
	while (sorted_env)
	{
		i = 0;
		ft_printf("declare -x ");
		while (((char *)sorted_env->content)[i] != '=')
			ft_printf("%c", ((char *)sorted_env->content)[i++]);
		if (ft_strchr(sorted_env->content, '='))
			ft_printf("=\"%s\"", ft_strchr(sorted_env->content, '=') + 1);
		ft_printf("\n");
		sorted_env = sorted_env->next;
	}
	ft_lstclear(&sorted_env, free);
}

void	print_export_error(char *str)
{
	ft_putstr_fd("minicecco: export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

int	export_builtin(char **argv, t_list **env)
{
	int		i;
	int		exit_status;
	char	*key;

	if (!argv[1])
		print_export(*env);
	exit_status = 0;
	i = 0;
	while (argv[++i])
	{
		key = get_key(argv[i]);
		if (is_valid_identifier(key) && !(key[0] == '_' && key[1] == '\0'))
		{
			if (ft_strchr(argv[i], '=') != NULL)
				lst_remove_key(env, key);
			ft_lstadd_back(env, ft_lstnew(ft_strdup(argv[i])));
		}
		else if (!(key[0] == '_' && key[1] == '\0'))
		{
			print_export_error(argv[i]);
			exit_status = 1;
		}
		free(key);
	}
	return (exit_status);
}
