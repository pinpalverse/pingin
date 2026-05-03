all:
	gcc main.c -g -o out -Wall -Wextra

debug:
	gcc main.c -fsanitize=address -g -fno-omit-frame-pointer -o out -Wall -Wextra

clean:
	rm out