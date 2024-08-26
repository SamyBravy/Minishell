/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:40:41 by marvin            #+#    #+#             */
/*   Updated: 2024/08/13 19:40:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*unique_name(void)
{
	static int	n = 0;
	char		*str;
	char		*tmp;

	tmp = ft_itoa(n);
	str = ft_strjoin(".tmp/.", tmp);
	free(tmp);
	n++;
	return (str);
}

static void	read_heredoc(int fd, char *eof)
{
	char	*line;

	if (eof[0] == '\x1E')
		eof[0] = '\0';
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (g_signal == SIGINT || !line
			|| (!ft_strncmp(line, eof, ft_strlen(eof))
				&& line[ft_strlen(eof)] == '\n'))
		{
			if (line)
				free(line);
			else if (g_signal != SIGINT)
				ft_printf("minicecco: warning: \
here-document delimited by end-of-file (wanted `%s')\n", eof);
			break ;
		}
		// espandi variabili
		ft_putstr_fd(line, fd);
		free(line);
	}
	get_next_line(-fd);
}

void	create_heredocs(t_input *input)
{
	char	*eof;

	while (input)
	{
		if (input->type == HEREDOC)
		{
			eof = input->str;
			while (input->fd < 0)
			{
				input->str = unique_name();
				input->fd = open(input->str, O_CREAT | O_EXCL | O_WRONLY, 0644);
				if (input->fd == -1)
					free(input->str);
			}
			read_heredoc(input->fd, eof);
			free(eof);
			close(input->fd);
		}
		input = input->next;
	}
}
