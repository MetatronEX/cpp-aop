cpp11:
	g++ -std=c++0x -I./c++0x -o test_aop test_aop.cpp

cpp99:
	g++ -DCPP99 -I./c++99 -o test_aop test_aop.cpp

clean:
	rm test_aop
