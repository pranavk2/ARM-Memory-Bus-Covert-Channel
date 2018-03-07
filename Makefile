all:
	g++ -std=c++0x -O0 -g reader.cpp -o reader 
	g++ -std=c++0x -O0 -g sender.cpp -o sender 

sender:
	g++ -std=c++0x -O0 -g sender.cpp -o sender


reader:
	g++ -std=c++0x -O0 -g reader.cpp -o reader


clean:
	rm reader sender temp.txt
