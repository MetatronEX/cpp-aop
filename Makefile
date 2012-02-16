
all:
	g++ -std=c++0x -I./c++0x -o test_aop test_aop.cpp

clean:
	rm test_aop
