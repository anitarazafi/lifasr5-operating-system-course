all: echoserver echoclient

echoserver: echoserver.cpp socklib.h socklib.cpp
	g++ -Wall -o echoserver echoserver.cpp socklib.cpp

echoclient: echoclient.cpp socklib.h socklib.cpp
	g++ -Wall -o echoclient echoclient.cpp socklib.cpp

clean:
	rm -rf *~ *.o echoserver echoclient
