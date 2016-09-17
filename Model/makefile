cc= g++ -std=c++11

all : run main.o network.o clean

run : main.o network.o 
	$(cc) main.o network.o -o ../modeling
main.o : main.cpp network.hpp
	$(cc) -c main.cpp
network.o : network.cpp network.hpp 
	$(cc) -c network.cpp

clean:
	rm -rf *.o
