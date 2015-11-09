SRSC = 3dp.cc
OBJS = 3dp.o
CXX = g++
CXXFLAGS = -g -Wall
conc:$(SRSC)
	$(CXX) $(CXXFLAGS) -o 3dp $(SRSC)
clean:
	 rm -f $(OBJS)
depend:
	 makedepend -Y $(SRCS)
