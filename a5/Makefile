CXXFLAGS = g++ -pedantic -Wall -Werror -Wextra -g -std=c++14

rv32i: main.o memory.o hex.o rv32i_decode.o registerfile.o rv32i_hart.o cpu_single_hart.o
	$(CXXFLAGS) -o rv32i rv32i_decode.o main.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o

main.o: main.cpp
	$(CXXFLAGS) -c -o main.o main.cpp

memory.o: memory.cpp
	$(CXXFLAGS) -c -o memory.o memory.cpp

hex.o: hex.cpp
	$(CXXFLAGS) -c -o hex.o hex.cpp

rv32i_decode.o: rv32i_decode.cpp
	$(CXXFLAGS) -c -o rv32i_decode.o rv32i_decode.cpp

registerfile.o: registerfile.cpp
	$(CXXFLAGS) -c -o registerfile.o registerfile.cpp

rv32i_hart.o: rv32i_hart.cpp
	$(CXXFLAGS) -c -o rv32i_hart.o rv32i_hart.cpp

cpu_single_hart.o: cpu_single_hart.cpp
	$(CXXFLAGS) -c -o cpu_single_hart.o cpu_single_hart.cpp

clean:
	rm rv32i *.o *.run