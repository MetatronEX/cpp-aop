cpp11:
	g++ -std=c++0x -I./cpp0x -o test_aop test_aop.cpp

cpp98:
	g++ -DCPP98 -I./cpp98 -o test_aop test_aop.cpp

clean:
	rm test_aop
