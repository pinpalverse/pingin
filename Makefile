all:
	gcc main.c -g -o out

tests:
	gcc -o conf_test tests/conf_test.c -Wall -Wextra

clean:
	rm out