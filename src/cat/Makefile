CC=gcc
FLAGS=-Wextra -Werror -Wall -std=c11 -o
NAME=s21_cat

.PHONY : clean rebuild

all : 
	$(CC) $(FLAGS) $(NAME) s21_cat.c

clean :
	rm $(NAME)

rebuild :
	make clean && make all

clang :
	clang-format --style=Google -n *.c *.h
	clang-format --style=Google -i *.c *.h