a.out: main.cpp logger.h.gch pub_sub.h.gch
	g++ main.cpp -pthread
	
pub_sub.h.gch: pub_sub.h
	g++ pub_sub.h

logger.h.gch: logger.h
	g++ logger.h

clean:
	rm -f *.o *.gch *.out