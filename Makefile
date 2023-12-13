a.out: main.cpp wheel.o gambler.o logger.o betting_strategy.o
	g++ main.cpp wheel.o gambler.o logger.o betting_strategy.o -pthread
	
wheel.o: wheel.cpp logger.o wheel.h
	g++ -c wheel.cpp -o wheel.o
	
gambler.o: gambler.cpp logger.o gambler.h
	g++ -c gambler.cpp -o gambler.o
	
betting_strategy.o: betting_strategy.cpp logger.o betting_strategy.h
	g++ -c betting_strategy.cpp -o betting_strategy.o
	
logger.o: logger.cpp logger.h
	g++ -c logger.cpp -o logger.o
	
test: test.cpp
	g++ test.cpp -o test.o
	
clean:
	rm -f *.o *.gch *.out
