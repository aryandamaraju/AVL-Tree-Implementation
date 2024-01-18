CXX = g++
CXXFLAGS = -Wall -g

proj2: show.o mytest.cpp
	$(CXX) $(CXXFLAGS) show.o mytest.cpp -o proj2

show.o: show.cpp show.h
	$(CXX) $(CXXFLAGS) -c show.cpp

run:
	./proj2

val1:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./proj2

clean:
	rm ~
	rm.o
