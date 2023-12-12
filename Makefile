a.out: main.cpp wheel.o gambler.o logger.o
	g++ main.cpp wheel.o gambler.o logger.o -pthread
	
wheel.o: wheel.cpp wheel.h logger.o
	g++ -c wheel.cpp -o wheel.o
	
gambler.o: gambler.cpp gambler.h logger.o
	g++ -c gambler.cpp -o gambler.o
	
logger.o: logger.cpp logger.h
	g++ -c logger.cpp -o logger.o
	
clean:
	rm -f *.o *.gch *.out
