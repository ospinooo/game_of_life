default:

compile: game_of_life.c
	gcc -o game_of_life.o game_of_life.c

run: compile game_of_life.c
	./game_of_life.o