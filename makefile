all:
	clang -Wall -Wextra -Werror -g3 -ggdb \
	get_next_line/get_next_line_utils.c get_next_line/get_next_line.c test.c -o test