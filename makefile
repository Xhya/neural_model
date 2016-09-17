cc = g++ -std=c++14
model_path = Model
interface_path = Interface

all : run modeling main.o network.o clean


run : $(interface_path)/main.cpp $(interface_path)/window.cpp $(interface_path)/window.hpp modeling
	$(cc) $(interface_path)/main.cpp $(interface_path)/window.cpp `pkg-config gtkmm-3.0 --cflags --libs` -o run

modeling : main.o network.o
	$(cc) $(model_path)/main.o $(model_path)/network.o -o modeling
main.o : $(model_path)/main.cpp network.o
	$(cc) -c $(model_path)/main.cpp -o $(model_path)/main.o
network.o : $(model_path)/network.cpp $(model_path)/network.hpp 
	$(cc) -c $(model_path)/network.cpp -o $(model_path)/network.o

clean:
	rm -rf $(model_path)/*.o
