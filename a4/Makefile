CXXFLAGS = g++ -pedantic -Wall -Werror -Wextra -g -std=c++14

rv32i: main.o memory.o hex.o rv32i_decode.o
	$(CXXFLAGS) -o rv32i rv32i_decode.o main.o memory.o hex.o

main.o: main.cpp
	$(CXXFLAGS) -c -o main.o main.cpp

memory.o: memory.cpp
	$(CXXFLAGS) -c -o memory.o memory.cpp

hex.o: hex.cpp
	$(CXXFLAGS) -c -o hex.o hex.cpp

rv32i_decode.o: rv32i_decode.cpp
	$(CXXFLAGS) -c -o rv32i_decode.o rv32i_decode.cpp

clean:
	rm rv32i *.o *.run