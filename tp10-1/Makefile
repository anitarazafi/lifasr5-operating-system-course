all: echoserver
echoserver: echoserver.cpp socklib.h socklib.cpp recv_line.h recv_line.cpp
	g++ -Wall -o echoserver echoserver.cpp socklib.cpp recv_line.cpp
clean:
	rm -rf *~ *.o echoserver