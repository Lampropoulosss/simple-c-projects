

CC = gcc

main: main.c
	@if [ ! -d "bin" ]; then mkdir bin; fi
	$(CC) main.c fileUtils.c -o bin/main

clean:
	rm -f -r main todo_list.csv bin

run: main
	./bin/main

