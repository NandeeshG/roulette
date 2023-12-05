a.out: main.cpp wheel.o gambler.o
	g++ main.cpp wheel.o gambler.o -pthread
	
wheel.o: wheel.cpp
	g++ -c wheel.cpp -o wheel.o
	
gambler.o: gambler.cpp
	g++ -c gambler.cpp -o gambler.o
	
clean:
	rm -f *.o *.gch *.out