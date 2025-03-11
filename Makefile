# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 13:55:42 by abessa-m          #+#    #+#              #
#    Updated: 2025/03/11 10:32:43 by abessa-m         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
NAME_BONUS	:= minishell
LIBFT		:= libft/libft.a
LIBFT_DIR	:= libft
##################################################################### Compiler #
CC			= cc
CFLAGS		= -Wall -Wextra 
CFLAGS		+= -Werror

CFLAGS		+= -g
#CFLAGS		+= -fsanitize=address -fsanitize=leak
########################################################### Intermidiate steps #
RM			:= rm -f
AR			:= ar rcs
########################################################## Objects and Headers #
#HEADERS		:= minishell.h
#INCLUDES	:= minishell.h
INCLUDES	:= -I./include

SRCS		:= \
	./playground/practice00.c 
OBJS		:= $(SRCS:.c=.o)

#SRCS_BONUS	:= 
#OBJS_BONUS	:= $(SRCS_BONUS:.c=.o)
###################################################################### Targets #
all: $(NAME) #bonus

$(NAME): $(LIBFT) $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) \
	&& echo "$(GRAY)Compiled:$(COR) $(SRCS)"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make --silent --no-print-directory -C $(LIBFT_DIR)

libft : $(LIBFT)

#bonus: $(NAME)
#
#bonus: $(OBJS_BONUS) $(LIBFT)
#	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $(NAME) \
#	&& echo "$(GRAY)Compiled:$(COR) $(SRCS_BONUS)"

clean:
	@$(RM) *.o *.gch *.exe $(OBJS) $(OBJS_BONUS)\
	;make --silent --no-print-directory -C $(LIBFT_DIR) clean 

fclean: clean
	@make --silent --no-print-directory -C $(LIBFT_DIR) fclean \
	&&$(RM) $(NAME) $(NAME_BONUS)

re: fclean all
	@echo "$(GRAY)redone$(COR)"

.PHONY: all clean fclean re
####################################################################### Colors #
COR		:= \033[0m# COlor Remove
PURPLE	:= \033[1;35m# purple
GRAY	:= \033[1;90m# gray
YELLOW	:= \033[1;93m# yellow
######################################################################### Test #
#Recomendation: alias t="make test"
test: all clean
	@valgrind --quiet -s ./minishell ; \
	echo "$(COR)$(GRAY)Return value: $$?$(COR)" ; \
	echo -n "$(YELLOW)" ; \
	norminette | grep -v -E \
	"Too many functions in file|Comment is invalid in this scope|Empty line in \
	function|Consecutive newlines|Space on empty line" \
	| grep Error ; echo -n "$(COR)" 
