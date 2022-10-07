OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

CFLAGS= -std=c++98 `sdl-config --cflags`
LDFLAGS= `sdl-config --libs`
PROGRAM=lostmasterpiece

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -c -o $@

$(PROGRAM): $(OBJ)
	$(CXX) -o $(PROGRAM)  $(OBJ) $(LDFLAGS)

all: $(PROGRAM)

.PHONY: clean clear
clean:
	rm -f $(PROGRAM)  *.o


