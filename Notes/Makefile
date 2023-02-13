CXXFLAGS = -Wall -Werror -std=c++11

test: test.o
	g++ $(CXXFLAGS) -o test test.o

test.o: test.cpp
	g++ $(CXXFLAGS) -c test.cpp

clean:
	rm -f *.o test