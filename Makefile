CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -lpthread

all: client1 server client2

client1: client1_sender.cpp error_detection.h
	$(CXX) $(CXXFLAGS) -o client1 client1_sender.cpp $(LDFLAGS)

server: server.cpp error_detection.h error_injection.h
	$(CXX) $(CXXFLAGS) -o server server.cpp $(LDFLAGS)

client2: client2_receiver.cpp error_detection.h
	$(CXX) $(CXXFLAGS) -o client2 client2_receiver.cpp $(LDFLAGS)

clean:
	rm -f client1 server client2 *.o

.PHONY: all clean

