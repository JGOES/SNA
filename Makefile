SNA : main.cpp SNA.o
	g++ SNA.o main.cpp -o SNA -std=c++11

SNA.o : SNA.hpp SNA.cpp
	g++ SNA.hpp SNA.cpp -c -std=c++11

all : SNA

clean : 
	rm *.o *gch SNA

documentation : 
	mkdir doc
	doxygen doxygen.conf

