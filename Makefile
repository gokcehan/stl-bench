build:
	$(CXX) main.cc -std=c++11 -Wall -Wextra -ljsoncpp -O3 -o bench

clean:
	rm bench
