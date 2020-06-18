all: main.exe

CC = g++
INCLUDE = .
CFLAGS = -Wall -Wextra -std=gnu++14

main.exe: main.o MyAI.o
	$(CC) -o main.exe main.o MyAI.o
main.o: main.cpp 
	$(CC) -I$(INCLUDE) $(CFLAGS) -c main.cpp
MyAI.o: MyAI.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c MyAI.cpp
# State.o: State.cpp State.h
# 	$(CC) -I$(INCLUDE) $(CFLAGS) -c State.cpp


clean:
	rm -f main.exe main.o MyAI.o State.o
