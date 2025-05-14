# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 13:55:42 by abessa-m          #+#    #+#              #
#    Updated: 2025/04/17 14:18:34 by abessa-m         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
LIBFT		:= libft/libft.a
LIBFT_DIR	:= libft
##################################################################### Compiler #
CC			= cc
CFLAGS		+= -Wall -Wextra
#CFLAGS		+= -Werror
READFLAGS	= -lreadline

CFLAGS		+= -g
#CFLAGS		+= -D DEBUG=1
#CFLAGS		+= -fsanitize=address -fsanitize=leak
########################################################### Intermidiate steps #
RM			:= rm -f
AR			:= ar rcs
########################################################## Objects and Headers #
INCLUDES	:= -I./include

SRCS		:=	\
	src/minishell.c															\
	src/execution/exe.c														\
	src/execution/signal.c													\
	src/parser/parser.c														\
	src/parser/lexer.c														\
	src/parser/lexer-list.c													\
	src/parser/lexer-tokenizer.c											\
	src/parser/ast.c														\
	src/parser/ast-free.c													\
	src/parser/ast-parse-cmd.c												\
	src/parser/ast-parse-cmd2.c												\
	src/parser/ast-print.c													\
	src/parser/ast-redirect.c												\
	src/utils/ft_isspace.c													\
	src/utils/ft_malloc.c													\
	src/builtins/cd.c														\
	src/builtins/clear.c													\
	src/builtins/echo.c														\
	src/builtins/env.c														\
	src/builtins/exit.c														\
	src/builtins/export.c													\
	src/builtins/pwd.c														\
	src/builtins/unset.c													\
	src/builtins/check_builtins.c											\
	src/builtins/init_utils.c

OBJS		:= $(SRCS:.c=.o)

#SRCS_BONUS	:=
#OBJS_BONUS	:= $(SRCS_BONUS:.c=.o)
###################################################################### Targets #
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS) supp_doc
	@\
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(READFLAGS)


%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@							&&	\
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
	" > .readline.txt

bonus: $(NAME)

debug:
	@\
	$(MAKE) --silent fclean												;	\
	$(MAKE) --silent all CFLAGS+=-D\ DEBUG=1							&&	\
	$(MAKE) --silent clean

clean:
	@\
	make --silent --no-print-directory -C $(LIBFT_DIR) clean			;	\
	$(RM) *.o include/*.gch *.exe $(OBJS) $(OBJS_BONUS) 				;	\
	$(RM) ./*sync-conf*													;	\
	$(RM) ./*/*sync-conf*												;	\
	$(RM) ./*/*/*sync-conf*												;	\
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
test:
	@\
	$(MAKE) --silent fclean												;	\
	$(MAKE) --silent all CFLAGS+=-D\ DEBUG=1							&&	\
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
		norminette src/ include/ playground/								\
		| grep -v OK 														\
		| grep -v WRONG_SCOPE_COMMENT										\
		| grep -v EMPTY_LINE_FUNCTION										\
		| grep -v TOO_MANY_FUNCS										;	\
	echo -n "$(COR)"

#	\
#	tail -n +4 log.txt													|	\
#	awk '{																	\
#	gsub(/^==[0-9]*== /, "")											;	\
#	gsub(/^--[0-9]*-- /, "")											;	\
#	gsub(/^used_suppression: /, "")										;	\
#	if (length($0) > 0) print $0											\
#	}'																	;	\

#		--log-file=log.txt													\
