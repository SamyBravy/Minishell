/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:34:57 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/11 18:01:33 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{	
	t_input *input = malloc(sizeof(t_input));
	input->type = CMD;
	input->str = ft_strdup("ls -l");
	input->next = NULL;
	input->fd = -1;

	t_input *input1 = malloc(sizeof(t_input));
	input1->type = INPUT;
	input1->str = ft_strdup("input.txt");
	input1->next = NULL;

	t_input *input2 = malloc(sizeof(t_input));
	input2->type = PIPE;
	input2->str = NULL;
	input2->next = NULL;
	input2->fd = -1;

	t_input *input22 = malloc(sizeof(t_input));
	input22->type = CMD;
	input22->str = ft_strdup("wc -l");
	input22->next = NULL;

	t_input *input3 = malloc(sizeof(t_input));
	input3->type = PIPE;
	input3->str = NULL;
	input3->next = NULL;
	input3->fd = -1;

	t_input *input4 = malloc(sizeof(t_input));
	input4->type = CMD;
	input4->str = ft_strdup("cdat");
	input4->next = NULL;

	t_input *input5 = malloc(sizeof(t_input));
	input5->type = HEREDOC;
	input5->str = ft_strdup("EOF");
	input5->next = NULL;

	input->next = input1;
	input1->next = input2;
	input2->next = input22;
	input22->next = input3;
	input3->next = input4;
	input4->next = input5;

	FILE *file = fopen("input.txt", "w");
	
	fclose(file);

	int exit_status = 0;
	executer(input, &env, &exit_status);

	unlink("input.txt");

	(void) argc;
	(void) argv;
	(void) env;
	return (0);
}
