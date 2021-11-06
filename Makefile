all:
	gcc -o runner runner.c -pthread -lm
	./runner
run:
	./runner