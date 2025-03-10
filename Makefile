# CCFLAGS=--std=c++0x -pedantic -Wall -Werror -g -ggdb3
CCFLAGS=--std=c++0x -pedantic -Wall -g -ggdb3
cacheSim: cacheSim.o BaseCache.o
	g++ -o cacheSim $(CCFLAGS) cacheSim.o BaseCache.o
%.o: %.cc BaseCache.h
	g++ -c $(CCFLAGS) $<
clean:
	rm -f *.o  *~ cacheSim
