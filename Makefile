all: exec

Cyfighter: Cyfighter.c
	gcc -o Cyfighter Cyfighter.c

exec: Cyfighter
	./Cyfighter