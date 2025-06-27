SRCS 	= minishell.c banner.c signals.c garbage_collector.c lexer_pt_one.c lexer_pt_two.c lexer_pt_three.c lexer_pt_four.c \
			parser_pt_one.c parser_pt_two.c utils_pt_one.c utils_pt_two.c executor_pt_one.c \
			built_in_pt_one.c built_in_pt_two.c \
			z_temp_funcs.c
LIBS	= -lreadline
NAME	=	minishell
CC		=	cc
CFLAGS	=	#-Wall -Wextra -Werror
RM		=	rm -rf

all: $(NAME)

$(NAME): $(SRCS)
	${CC} ${CFLAGS} -o $(NAME) $(SRCS) $(LIBS)

clean:
	$(RM) $(NAME)

fclean:	clean

re:		clean all

.PHONY:	all clean fclean re