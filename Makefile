a.out: main.cpp logger.h.gch pub_sub.h.gch wheel.o gambler.o
	g++ main.cpp wheel.o gambler.o -pthread
	
wheel.o: wheel.cpp logger.h.gch pub_sub.h.gch
	g++ -c wheel.cpp -o wheel.o
	
gambler.o: gambler.cpp logger.h.gch pub_sub.h.gch
	g++ -c gambler.cpp -o gambler.o
	
pub_sub.h.gch: pub_sub.h
	g++ pub_sub.h

logger.h.gch: logger.h
	g++ logger.h

clean:
	rm -f *.o *.gch *.out