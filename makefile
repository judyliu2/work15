all: sem.c
	gcc -o control sem.c
	gcc -o main main.c

clean:
	@rm *.o
	@rm *~

