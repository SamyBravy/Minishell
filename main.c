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
	input->type = HEREDOC;
	input->str = ft_strdup("here");
	input->next = NULL;
	
	t_input *inputt = malloc(sizeof(t_input));
	inputt->type = HEREDOC;
	inputt->str = ft_strdup("heree");
	inputt->next = NULL;
	
	t_input *input1 = malloc(sizeof(t_input));
	input1->type = CMD;
	input1->str = ft_strdup("ls");
	input1->next = NULL;

	t_input *input2 = malloc(sizeof(t_input));
	input2->type = PIPE;
	input2->str = NULL;
	input2->next = NULL;

	t_input *input3 = malloc(sizeof(t_input));
	input3->type = INPUT;
	input3->str = ft_strdup("input.txt");
	input3->next = NULL;

	t_input *input4 = malloc(sizeof(t_input));
	input4->type = TRUNC;
	input4->str = ft_strdup("output.txt");
	input4->next = NULL;

	t_input *input5 = malloc(sizeof(t_input));
	input5->type = APPEND;
	input5->str = ft_strdup("append.txt");
	input5->next = NULL;

	t_input *input6 = malloc(sizeof(t_input));
	input6->type = INPUT;
	input6->str = ft_strdup("inpurt.txt");
	input6->next = NULL;


	input->next = inputt;
	inputt->next = input1;
	input1->next = input2;
	input2->next = input3;
	input3->next = input4;
	input4->next = input5;
	input5->next = input6;

	FILE *file = fopen("input.txt", "w");

	executer(input);

	fclose(file);

	unlink("output.txt");
	unlink("append.txt");
	unlink("input.txt");

	(void) argc;
	(void) argv;
	(void) env;
	return (0);
}
