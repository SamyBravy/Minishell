/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:44:30 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:44:30 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	change_directory(char *new_path, char *arg, t_list **env)
{
	char	cwd[PATH_MAX];
	char	*tmp;

	getcwd(cwd, sizeof(cwd));
	if (chdir(new_path) == -1)
	{
		ft_putstr_fd("minicecco: cd: ", STDERR_FILENO);
		perror(new_path);
		free(new_path);
		return (1);
	}
	free(new_path);
	if (arg && !ft_strcmp(arg, "-"))
		ft_printf("%s\n", ft_getenv("OLDPWD", *env));
	tmp = ft_strjoin("OLDPWD=", cwd);
	if (ft_getenv("OLDPWD", *env) != NULL)
		ft_export(tmp, env, 0);
	free(tmp);
	getcwd(cwd, sizeof(cwd));
	tmp = ft_strjoin("PWD=", cwd);
	if (ft_getenv("PWD", *env) != NULL)
		ft_export(tmp, env, 0);
	free(tmp);
	return (0);
}

int	cd_builtin(char **argv, t_list **env)
{
	char	*new_path;

	if (argv[1] != NULL && argv[2] != NULL)
		return (ft_putstr_fd("minicecco: cd: too many arguments\n", 2), 1);
	if (argv[1] == NULL && ft_getenv("HOME", *env) != NULL)
		new_path = ft_strdup(ft_getenv("HOME", *env));
	else if (!argv[1] && !ft_getenv("HOME", *env))
		return (ft_putstr_fd("minicecco: cd: HOME not set\n", 2), 1);
	else if (argv[1][0] == '\x1E')
		return (0);
	else if (!ft_strcmp(argv[1], "-"))
	{
		if (!ft_getenv("OLDPWD", *env) || ft_getenv("OLDPWD", *env)[0] == '\0')
			return (ft_putstr_fd("minicecco: cd: OLDPWD not set\n", 2), 1);
		new_path = ft_strdup(ft_getenv("OLDPWD", *env));
	}
	else if ((argv[1][0] == '~' && (argv[1][1] == '/' || argv[1][1] == '\0'))
		&& ft_getenv("HOME", *env) != NULL)
		new_path = ft_strjoin(ft_getenv("HOME", *env), argv[1] + 1);
	else
		new_path = ft_strdup(argv[1]);
	return (change_directory(new_path, argv[1], env));
}
