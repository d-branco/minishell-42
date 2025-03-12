# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 13:55:42 by abessa-m          #+#    #+#              #
#    Updated: 2025/03/12 16:22:04 by abessa-m         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
LIBFT		:= libft/libft.a
LIBFT_DIR	:= libft
##################################################################### Compiler #
CC			= cc
CFLAGS		+= -Wall -Wextra
CFLAGS		+= -Werror
READFLAGS	= -lreadline

CFLAGS		+= -g
CFLAGS		+= -D DEBUG=1 # Comment before deliverance
#CFLAGS		+= -fsanitize=address -fsanitize=leak
########################################################### Intermidiate steps #
RM			:= rm -f
AR			:= ar rcs
########################################################## Objects and Headers #
INCLUDES	:= -I./include

SRCS		:= \
	playground/practice01.c
OBJS		:= $(SRCS:.c=.o)

#SRCS_BONUS	:=
#OBJS_BONUS	:= $(SRCS_BONUS:.c=.o)
###################################################################### Targets #
all: $(NAME) #bonus

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(READFLAGS) \
	&& echo "$(GRAY)File compiled:$(COR)	$(SRCS)"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make --silent --no-print-directory -C $(LIBFT_DIR) \
	&& echo "$(PURPLE)Library built:$(COR)	$(LIBFT)"

libft : $(LIBFT)

#bonus: $(NAME)
#
#bonus: $(OBJS_BONUS) $(LIBFT)
#	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $(NAME) $(READFLAGS) \
#	&& echo "$(GRAY)Compiled:$(COR) $(SRCS_BONUS)"

clean:
	@$(RM) *.o *.gch *.exe $(OBJS) $(OBJS_BONUS)\
	;make --silent --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@make --silent --no-print-directory -C $(LIBFT_DIR) fclean \
	&&$(RM) $(NAME)

re: fclean all
	@echo "$(GRAY)redone$(COR)"

.PHONY: all clean fclean re
####################################################################### Colors #
COR		:= \033[0m# COlor Remove
PURPLE	:= \033[1;35m# purple
GRAY	:= \033[1;90m# gray
YELLOW	:= \033[1;93m# yellow
######################################################################### Test #
#Recomendation to define alias t="make test"
test: all clean
	@echo "$(COR)$(GRAY)\
	========================================== $(NAME) START\
	$(COR)" ; \
	\
	valgrind \
		--track-fds=yes \
		--show-error-list=yes \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		\
		--suppressions=readline.supp \
		--log-file=log.txt \
		\
		./minishell ; \
	\
	echo "$(COR)$(GRAY)\
	========================================== $(NAME) END\n\
	Return value: $$?$(COR)" ; \
	\
	tail -n 12 log.txt | awk '{gsub(/^==[0-9]*== /, ""); \
	if (length($0) > 0) print $0}' ; \
	\
	echo -n "$(YELLOW)" ; \
		norminette \
		| grep -v OK \
		| grep -v WRONG_SCOPE_COMMENT \
		| grep -v GLOBAL_VAR_DETECTED \
		; echo -n "$(COR)"
