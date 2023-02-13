#
# PROGRAM: Lab Training Exercise
# AUTHOR:  Anthony Chappetto z1888754
#

CXXFLAGS = -Wall -Werror -std=c++11

labtrain: labtrain.o
	g++ $(CXXFLAGS) -o labtrain labtrain.o

labtrain.o: labtrain.cpp
	g++ $(CXXFLAGS) -c labtrain.cpp

clean:
	rm -f *.o labtrain