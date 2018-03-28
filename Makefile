all:
	g++ -msse4.1 -std=c++0x -O0 -g reader.cpp ../../papi-5.3.0/src/libpapi.a -o reader 
	g++ -msse4.1 -std=c++0x -O0 -g sender.cpp ../../papi-5.3.0/src/libpapi.a -o sender

clean:
	rm reader sender
