SRC		:= minishell.c \
			lexer/lexer_pt_one.c lexer/lexer_pt_two.c lexer/lexer_pt_three.c lexer/lexer_pt_four.c \
			lexer/lexer_pt_five.c lexer/lexer_pt_six.c lexer/lexer_pt_seven.c lexer/lexer_pt_eight.c \
			parser/parser_pt_one.c parser/parser_pt_two.c parser/parser_pt_three.c \
			parser/parser_pt_four.c parser/environment_parser.c \
			utils/utils_pt_one.c utils/utils_pt_two.c utils/utils_pt_three.c utils/utils_pt_four.c \
			utils/utils_pt_five.c utils/utils_pt_six.c utils/utils_pt_seven.c utils/utils_pt_eight.c \
			utils/garbage_collector.c utils/banner.c utils/signals.c \
			built_ins/built_in_pt_one.c built_ins/built_in_pt_two.c built_ins/built_in_pt_three.c \
			built_ins/built_in_pt_four.c built_ins/built_in_pt_five.c built_ins/built_in_pt_six.c \
			built_ins/built_in_pt_seven.c built_ins/built_in_pt_eight.c \
			executor/executor_pt_one.c executor/executor_pt_two.c executor/executor_pt_three.c \
			executor/executor_pt_four.c executor/executor_pt_five.c executor/executor_pt_six.c \
			executor/executor_pt_seven.c executor/executor_pt_eight.c
LIBS	:= -lreadline
NAME	:=	minishell
CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
RM		:=	rm -rf
OBJ_DIR := objects
OBJ		:= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
GREEN	:= \033[0;32m
BLUE	:= \033[0;34m
CYAN	:= \033[0;36m
RESET	:= \033[0m
BOLD	:= \033[1m

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(CYAN)Compiling:$(RESET) %-20s → $(GREEN)%s$(RESET)\n" "$<" "$@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@printf "$(BOLD)Linking...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)
	@printf "$(GREEN)✔ Build complete: $(NAME)$(RESET)\n"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(BLUE)🧹 Cleaned object files$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(BLUE)🧹 Removed binary $(NAME)$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re