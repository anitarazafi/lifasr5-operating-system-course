all: desc_blocage desc_sigpipe

desc_blocage: desc_blocage.cpp
	g++ -o desc_blocage desc_blocage.cpp -Wall

desc_sigpipe: desc_sigpipe.cpp
	g++ -o desc_sigpipe desc_sigpipe.cpp -Wall

clean:
	rm -f desc_blocage desc_sigpipe
