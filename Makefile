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

# valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet --tool=memcheck --keep-debuginfo=yes --track-fds=yes ./minishell
NAME = minishell
MY_LIB = ./my_lib
SRC = 	minishell.c parsing.c env_utils.c \
		\
		executer/executer.c executer/clean.c executer/handle_signals.c \
		executer/executer_utils.c executer/ft_heredocs.c executer/execute_command.c \
		\
		builtins/exit_builtin.c builtins/echo_builtin.c builtins/pwd_builtin.c builtins/env_builtin.c \
		builtins/unset_builtin.c builtins/export_builtin.c builtins/cd_builtin.c
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
	@echo "\e[0;91m[$(NAME)] deleted!\e[0m"

re: fclean all

.PHONY: all clean fclean re
