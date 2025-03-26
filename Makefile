# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 13:55:42 by abessa-m          #+#    #+#              #
#    Updated: 2025/03/26 17:42:48 by abessa-m         ###   ########.fr        #
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
#CFLAGS		+= -D DEBUG=1
#CFLAGS		+= -fsanitize=address -fsanitize=leak
########################################################### Intermidiate steps #
RM			:= rm -f
AR			:= ar rcs
########################################################## Objects and Headers #
INCLUDES	:= -I./include

SRCS		:= \
	src/minishell.c \
	src/cmd_interpreter.c \
	src/parser/parser.c 
#	playground/practice01.c
#	playground/practice00.c
OBJS		:= $(SRCS:.c=.o)

#SRCS_BONUS	:=
#OBJS_BONUS	:= $(SRCS_BONUS:.c=.o)
###################################################################### Targets #
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS) supp_doc
	@\
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(READFLAGS) &&	\
	echo "$(GRAY)File compiled:$(COR)	$(SRCS)"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR) &&	\
	echo "$(GRAY)Library built:$(COR)	$(LIBFT)"

libft : $(LIBFT)

supp_doc:
	@echo "\
	{\n\
		leak readline\n\
		Memcheck:Leak\n\
		...\n\
		fun:readline\n\
	}\n\
	" > .readline.txt

bonus: $(NAME)

clean:
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR) clean ;	\
	$(RM) *.o include/*.gch *.exe $(OBJS) $(OBJS_BONUS) &&		\
	$(RM) ./*sync-conf*											\
	$(RM) ./*/*sync-conf*										\
	$(RM) ./*/*/*sync-conf*										\
	echo "$(GRAY)Files cleaned.$(COR)"

fclean: clean
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR) fclean ;	\
	$(RM) $(NAME) &&											\
	echo "$(GRAY)File fcleaned.$(COR)"

re: fclean all
	@echo "$(GRAY)redone$(COR)"

.PHONY: all clean fclean re
####################################################################### Colors #
COR		:= \033[0m# COlor Remove
PURPLE	:= \033[1;35m# purple
GRAY	:= \033[1;90m# gray
YELLOW	:= \033[1;93m# yellow
######################################################################### Test #
test:
	@\
	$(MAKE) --silent fclean ;					\
	$(MAKE) --silent all CFLAGS+=-D\ DEBUG=1 &&	\
	$(MAKE) --silent clean &&					\
	\
	echo "$(COR)$(GRAY)========================================== $(NAME) START\
	$(COR)" && \
	\
	valgrind							\
		--track-fds=yes					\
		--show-error-list=yes			\
		--leak-check=full				\
		--show-leak-kinds=all			\
		--track-origins=yes				\
		--suppressions=.readline.txt	\
		--log-file=log.txt				\
										\
		./minishell ;					\
	\
	echo "$(COR)$(GRAY)========================================== $(NAME) END\n\
	$(COR)RETURN VALUE: $$?" && \
	\
	tail -n 18 log.txt |					\
	awk '{									\
	gsub(/^==[0-9]*== /, "") ;				\
	gsub(/^--[0-9]*-- /, "") ;				\
	gsub(/^used_suppression: /, "") ;		\
	if (length($0) > 0) print $0			\
	}' ;									\
	\
	echo -n "$(YELLOW)" ;						\
		norminette src/ include/ playground/	\
		| grep -v OK 							\
		| grep -v WRONG_SCOPE_COMMENT			\
		| grep -v GLOBAL_VAR_DETECTED			\
	; echo -n "$(COR)" ;						\
	