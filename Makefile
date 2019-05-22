CXX = g++
CXXFLAGS = -std=c++11 -w -O2

ODIR = ./obj

run: $(ODIR)
	$(CXX) -c -o ./obj/equation.o Equation/equation.cpp $(CXXFLAGS)
	$(CXX) -c -o ./obj/tester.o test/tester/tester.cpp $(CXXFLAGS)
	$(CXX) -c -o ./obj/main.o test/tester/main.cpp $(CXXFLAGS)
	$(CXX) -o $@ ./obj/equation.o ./obj/tester.o ./obj/main.o $(CXXFLAGS)

$(ODIR):
	mkdir $@

clean:
	rm -rf $(ODIR)
	rm run
