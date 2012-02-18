CXXFLAGS=-Wall -ansi -pedantic

cpp11: clean
	g++ $(CXXFLAGS) -std=c++0x -I./cpp0x -o test_aop cpp0x/test.cpp

cpp98: clean
	g++ $(CXXFLAGS) -I./cpp98 -o test_aop cpp98/test.cpp

clean:
	rm -f test_aop
