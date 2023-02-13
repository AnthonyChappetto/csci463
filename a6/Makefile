CXXFLAGS = -Wall -Werror -std=c++11

reduce: reduce.o
	g++ $(CXXFLAGS) -pthread -o reduce reduce.o

reduce.o: reduce.cpp
	g++ $(CXXFLAGS) -c reduce.cpp

clean:
	rm -f *.o reduce