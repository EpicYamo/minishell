SRCS 	= minishell.c banner_func.c signals.c lexer_pt_one.c utils_pt_one.c \
			parser_pt_one.c executor_pt_one.c utils_pt_two.c
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