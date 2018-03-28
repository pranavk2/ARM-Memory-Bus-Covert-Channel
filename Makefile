all:
	g++ -msse4.1 -std=c++0x -O0 -g reader.cpp -o reader 
	g++ -msse4.1 -std=c++0x -O0 -g sender.cpp -o sender

clean:
	rm reader sender
