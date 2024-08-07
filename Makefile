# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/16 19:17:18 by sdell-er          #+#    #+#              #
#    Updated: 2024/08/07 09:26:53 by sdell-er         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
MY_LIB = ./my_lib
SRC = main.c
FLAGS = -g -Wall -Wextra -Werror -lreadline -L$(MY_LIB) -lmy_lib
CC = cc

all: $(NAME)

$(NAME): $(SRC)
	@make -C $(MY_LIB) > /dev/null
	@$(CC) $(FLAGS) $(SRC) -o $(NAME)
	@echo "\e[0;93m[$(NAME)] compiled!\e[0m"

clean:

fclean: clean
	@make fclean -C $(MY_LIB) > /dev/null
	@rm -f $(NAME)
	@echo "\e[0;91m[$(NAME)] deleted!\e[0m"

re: fclean all

.PHONY: all clean fclean re
