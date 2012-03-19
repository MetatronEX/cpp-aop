CXXFLAGS=-Wall -pedantic

cpp11: clean
	g++ $(CXXFLAGS) -std=c++0x -I./cpp0x -o test_aop_cpp0x cpp0x/test.cpp

cpp98: clean
	g++ $(CXXFLAGS) -std=c++98 -I./cpp98 -o test_aop_cpp98 cpp98/test.cpp

all: cpp11 cpp98

clean:
	rm -f test_aop
