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
	input->str = ft_strjoin("unset\x1D", "PWD");
	input->next = NULL;
	input->fd = -1;

	t_input *input2 = malloc(sizeof(t_input));
	input2->type = PIPE;
	input2->str = NULL;
	input2->next = NULL;
	input2->fd = -1;

	t_input *input3 = malloc(sizeof(t_input));
	input3->type = CMD;
	input3->str = ft_strdup("env");	
	input3->next = NULL;
	input3->fd = -1;

	input->next = input2;
	input2->next = input3;


	FILE *file = fopen("input.txt", "w");
	fprintf(file, "This is a test input\n file.\n");
	fclose(file);

	int exit_status = 0;
	//int tmp = dup(STDIN_FILENO); // questo va fatto nel parsing
	//dup2(tmp, STDIN_FILENO); // questo va fatto nel parsing
	//close(tmp); // questo va fatto nel parsing
	t_list *lst_env = ft_matrix_to_lst(env);
	executer(&input, &lst_env, &exit_status);
	ft_lstclear(&lst_env, free);
	
	//close(STDIN_FILENO); // questo va fatto nel parsing
	printf("Exit status: %d\n", exit_status);

	unlink("input.txt");
	unlink("output.txt");

	(void) argc;
	(void) argv;
	return (0);
}
