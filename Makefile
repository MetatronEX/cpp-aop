cpp11: clean
	g++ -std=c++0x -I./cpp0x -o test_aop test_aop.cpp

cpp98: clean
	g++ -DCPP98 -I./cpp98 -o test_aop test_aop.cpp

clean:
	rm test_aop
