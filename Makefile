LNK_OPT = -g -std=c++11 -lsdl2 -lsdl2_mixer -lpthread

all:
	g++ -g */*.cpp *.cpp -o test $(LNK_OPT)

clean:
	rm -f *.o
	rm -f */*.o
	rm -f test