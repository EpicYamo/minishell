SRCS 	= minishell.c banner_func.c signals.c lexer.c
LIBS	= libraries/libft.a -lreadline
NAME	=	minishell
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -rf

all: $(NAME)

$(NAME): $(SRCS)
	${CC} ${CFLAGS} -o $(NAME) $(SRCS) $(LIBS)

clean:
	$(RM) $(NAME)

fclean:	clean

re:		clean all

.PHONY:	all clean fclean re