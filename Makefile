LNK_OPT = -std=c++11 -lsdl2 -lsdl2_mixer -lpthread

all:
	g++ */*.cpp *.cpp -o meditation $(LNK_OPT)

clean:
	rm -f *.o
	rm -f */*.o
	rm -f meditation