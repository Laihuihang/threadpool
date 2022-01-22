main : main.o threadpool.o
	g++ -o main main.o threadpool.o -lpthread -std=c++11
main.o : main.cc
	g++ -c main.cc
threadpool.o : threadpool.cc
	g++ -c threadpool.cc
clean:
	rm threadpool.o main.o main