CXXFLAGS = g++ -pedantic -Wall -Werror -Wextra -g -std=c++11

memsim: main.o memory.o hex.o
	$(CXXFLAGS) -o memsim main.o memory.o hex.o

main.o: main.cpp
	$(CXXFLAGS) -c -o main.o main.cpp

memory.o: memory.cpp
	$(CXXFLAGS) -c -o memory.o memory.cpp

hex.o: hex.cpp
	$(CXXFLAGS) -c -o hex.o hex.cpp

clean:
	rm memsim *.o *.run