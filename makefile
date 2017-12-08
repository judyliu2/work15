all: sem.c
	@gcc -o tester sem.c

clean:
	@rm *.o
	@rm *~

run: all
	@./tester -c 4
	@./tester -v
	@./tester -c 3
	@./tester -v
	@./tester -r
