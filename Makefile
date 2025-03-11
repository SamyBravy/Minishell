# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/16 19:17:18 by sdell-er          #+#    #+#              #
#    Updated: 2024/08/11 15:17:44 by sdell-er         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
MY_LIB = ./my_lib
SRC = 	main.c \
		General/env_utils.c General/handle_signals.c \
		Parsing/add_node.c Parsing/balanced.c Parsing/control_chars.c Parsing/expand_main.c Parsing/expansion_help.c \
		Parsing/expansion_help2.c Parsing/file_samu1.c Parsing/file_samu2.c Parsing/heredoc_and_quotes.c Parsing/history.c \
		Parsing/main2.c Parsing/more_pipes_and_quotes.c Parsing/remove_empty.c Parsing/remove_spaces.c \
		Parsing/syntax_error.c Parsing/tokenize.c \
		Executer/executer.c Executer/clean.c Executer/executer_utils.c Executer/ft_heredocs.c Executer/execute_command.c \
		Builtins/exit_builtin.c Builtins/echo_builtin.c Builtins/pwd_builtin.c Builtins/env_builtin.c \
		Builtins/unset_builtin.c Builtins/export_builtin.c Builtins/cd_builtin.c
FLAGS = -g -Wall -Wextra -Werror -lreadline
CC = cc

all: $(NAME)

$(NAME): $(SRC)
	@make -C $(MY_LIB) 2>&1 | grep -v 'Leaving directory' | grep -v 'Entering directory'
	@$(CC) $(SRC) -o $(NAME) -L$(MY_LIB) -lmy_lib $(FLAGS)
	@echo "\e[0;93m[$(NAME)] compiled!\e[0m"

clean:
	@make clean -C $(MY_LIB) 2>&1 | grep -v 'Entering directory' | grep -v 'Leaving directory'

fclean:
	@make fclean -C $(MY_LIB) 2>&1 | grep -v 'Entering directory' | grep -v 'Leaving directory'
	@rm -f $(NAME)
	@rm -f .history
	@echo "\e[0;91m[$(NAME)] deleted!\e[0m"

val: all
	clear
	valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck --keep-debuginfo=yes --track-fds=yes ./minishell

re: fclean all

.PHONY: all clean fclean re
