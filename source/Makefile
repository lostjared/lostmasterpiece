SDLPATH=/usr/local/bin


all: main.o mxfont.o pixelbuffer.o
	${CXX} main.o mxfont.o pixelbuffer.o  -o lostmasterpiece  `sdl-config --libs`

main.o: main.cpp
	${CXX} main.cpp mxfont.cpp pixelbuffer.cpp  -c `sdl-config --cflags`

clean:
	rm -f *.o *~ lostmasterpiece
