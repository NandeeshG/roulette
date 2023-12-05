a.out: main.cpp logger.h.gch
	g++ main.cpp

logger.h.gch: logger.h
	g++ logger.h

clean:
	rm -f *.o *.gch *.out