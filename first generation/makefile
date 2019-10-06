objects = $(wildcard *.o)

all: client.o clientMain.o server.o serverMain.o common.o
	g++ -o client client.o clientMain.o common.o
	g++ -o server server.o serverMain.o common.o

client.o: client.cpp client.h common.h
	g++ -o client.o -c client.cpp

server.o: server.cpp server.h common.h
	g++ -o server.o -c server.cpp

common.o: common.cpp common.h
	g++ -o common.o -c common.cpp

.PHONY: clean
clean:
	rm -rf $(objects) server client