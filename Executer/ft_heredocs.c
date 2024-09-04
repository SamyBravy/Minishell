/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:43:37 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:43:37 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*unique_name(char *eof)
{
	static int	n = 0;
	char		*str;
	char		*tmp;

	tmp = ft_itoa(n);
	if (ft_strlen(eof) == 0 || ft_strlen(eof) > 1000)
		str = ft_strjoin(".", tmp);
	else
		str = ft_strjoin3(".", eof, tmp);
	free(tmp);
	n++;
	return (str);
}

static void	expand_line(char **line, t_list *env, int expand)
{
	char	*tmp;

	if (expand)
	{
		tmp = expand_samu(*line, env);
		free(*line);
		*line = tmp;
	}
}

static void	read_heredoc(int fd, char *eof, t_list *env, int expand)
{
	char	*line;
	char	*tmp;

	if (eof[0] == '\x1E')
		eof[0] = '\0';
	while (1)
	{
		tmp = ft_strjoin3("\033[0;36m", eof, "\033[0m> ");
		line = readline(tmp);
		free(tmp);
		if (g_signal == SIGINT || !line
			|| (!ft_strncmp(line, eof, ft_strlen(eof))
				&& line[ft_strlen(eof)] == '\0'))
		{
			if (line)
				free(line);
			else if (g_signal != SIGINT)
				ft_printf("minicecco: warning: \
here-document delimited by end-of-file (wanted `%s')\n", eof);
			break ;
		}
		expand_line(&line, env, expand);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

void	create_heredocs(t_input *input, t_list *env)
{
	char	*eof;
	int		expand;

	while (input)
	{
		if (input->type == HEREDOC)
		{
			eof = input->str;
			expand = 1;
			if (input->fd == -420)
				expand = 0;
			while (input->fd < 0)
			{
				input->str = unique_name(eof);
				input->fd = open(input->str, O_CREAT | O_EXCL | O_WRONLY, 0644);
				if (input->fd == -1)
					free(input->str);
			}
			read_heredoc(input->fd, eof, env, expand);
			free(eof);
			close(input->fd);
		}
		input = input->next;
	}
}
