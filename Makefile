# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 13:55:42 by abessa-m          #+#    #+#              #
#    Updated: 2025/07/03 18:34:25 by abessa-m         ###   ########.fr        #
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

#CFLAGS		+= -g
#CFLAGS		+= -g3
#CFLAGS		+= -D DEBUG=1
#CFLAGS		+= -fsanitize=address -fsanitize=leak
########################################################### Intermidiate steps #
RM			:= rm -f
AR			:= ar rcs
########################################################## Objects and Headers #
INCLUDES	:= -I./include

SRCS		:=	\
	src/minishell.c															\
																			\
	src/builtins/bridge.c													\
	src/builtins/cd.c														\
	src/builtins/check_builtins.c											\
	src/builtins/echo.c														\
	src/builtins/env.c														\
	src/builtins/env_utils-2.c												\
	src/builtins/env_utils-3.c												\
	src/builtins/env_utils.c												\
	src/builtins/exit.c														\
	src/builtins/export.c													\
	src/builtins/export_utils-2.c											\
	src/builtins/export_utils.c												\
	src/builtins/pwd.c														\
	src/builtins/replace_add_var.c											\
	src/builtins/unset.c													\
	src/builtins/unset_utils.c												\
																			\
	src/execution/parser.c													\
	src/execution/parser-0-lexer.c											\
	src/execution/parser-2.c												\
	src/execution/parser-3.c												\
	src/execution/parser-4.c												\
	src/execution/parser-5.c												\
	src/execution/parser-6-fd.c												\
	src/execution/parser-7-heredoc.c										\
	src/execution/parser-8.c												\
	src/execution/parser-9.c												\
	src/execution/parser-10.c												\
	src/execution/parser-11.c												\
	src/execution/parser-12.c												\
	src/execution/parser-13.c												\
	src/execution/parser-14.c												\
	src/execution/parser-15.c												\
	src/execution/parser-16.c												\
																			\
	src/expansion/dollar.c													\
	src/expansion/wildcards-2.c											\
	src/expansion/wildcards-3.c											\
	src/expansion/wildcards-4.c											\
	src/expansion/wildcards-5.c											\
	src/expansion/wildcards.c												\
																			\
	src/utils/error-0.c													\
	src/utils/ft_dprintf.c													\
	src/utils/ft_isspace.c													\
	src/utils/ft_malloc.c													\
	src/utils/ft_strcmp.c													\
	src/utils/init_n_etc.c													\
	src/utils/init_utils.c													\
	src/utils/signals.c

OBJS		:= $(SRCS:.c=.o)

#SRCS_BONUS	:=
#OBJS_BONUS	:= $(SRCS_BONUS:.c=.o)
###################################################################### Targets #
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS) supp_doc
	@\
	echo "$(GRAY)Compile flags:$(COR)	$(CC) $(CFLAGS)"				;	\
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(READFLAGS)


%.o: %.c
	@\
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@								&&	\
	echo "$(GRAY)File compiled:$(COR)	$<"

$(LIBFT):
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR)					&&	\
	echo "$(GRAY)Library built:$(COR)	$(LIBFT)"

libft : $(LIBFT)

supp_doc:
	@\
	echo "\
	{\n\
		leak readline\n\
		Memcheck:Leak\n\
		...\n\
		fun:readline\n\
	}\n\
	{\n\
		leak add_history\n\
		Memcheck:Leak\n\
		...\n\
		fun:add_history\n\
	}\n\
	" > .readline.txt

bonus: $(NAME)

debug:
	@\
	$(MAKE) --silent fclean												;	\
	$(MAKE) --silent all CFLAGS=\ -Wall\ -Wextra\ -D\ DEBUG=1			&&	\
	$(MAKE) --silent clean

clean:
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR) clean			;	\
	$(RM) *.o include/*.gch *.exe $(OBJS) $(OBJS_BONUS) 				;	\
	$(RM) ./a.out														;	\
	$(RM) ./playground/a.out											;	\
	echo "$(GRAY)Files cleaned.$(COR)"

fclean: clean
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR) fclean			;	\
	$(RM) $(NAME)														&&	\
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
test: supp_doc
	@\
	$(MAKE) --silent fclean												;	\
	$(MAKE) --silent all 												&&	\
	$(MAKE) --silent clean												&&	\
	\
	echo "\
	$(COR)$(GRAY)========================================== $(NAME) START\
	$(COR)" && \
	\
	valgrind																\
		--track-fds=yes														\
		--show-error-list=yes												\
		--leak-check=full													\
		--show-leak-kinds=all												\
		--track-origins=yes													\
		--suppressions=.readline.txt										\
	\
		./minishell														;	\
	\
	echo "\
	$(COR)$(GRAY)========================================== $(NAME) END\n\
	$(COR)RETURN VALUE: $$?"											&&	\
	\
	echo -n "$(YELLOW)" 												;	\
		norminette src/ include/											\
		| grep -v OK 														\
		| grep -v 'Setting locale to en_US'								;	\
	echo -n "$(COR)"													;	\
	echo -n "Error count: "												;	\
		norminette src/ include/ 											\
			| grep -v OK 													\
			| grep -v 'Setting locale to en_US'								\
			| grep -v Error!												\
		| wc -l

valgrind: supp_doc
	@\
	valgrind																\
		--track-fds=yes														\
		--show-error-list=yes												\
		--leak-check=full													\
		--show-leak-kinds=all												\
		--track-origins=yes													\
		--suppressions=.readline.txt										\
	\
		./minishell
