SRC = $(wildcard *.cpp)
OBJS = $(SRC:.cpp=.o)
DEPS = swing_sensor_data.h
CXX = g++
DEBUG = -g
CXXFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = $(DEBUG) -O2 -Wall 

DK : $(OBJS)
	$(CXX) -o DK $(OBJS) $(LFLAGS)

swing_sensor_data.o : $(DEPS) swing_sensor_data.cpp
	$(CXX) $(CXXFLAGS) swing_sensor_data.cpp

main.o : $(DEPS) main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

clean:
	\rm *.o *~ DK

