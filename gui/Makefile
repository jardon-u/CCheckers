SRC=play.cc board.cc game.cc cell.cc
PSRC=$(patsubst %,../src/%,$(SRC))
OBJ=$(SRC:.cc=.o)

all:
	swig -python -shadow -c++ cc.i
	g++ -c $(PSRC) cc_wrap.cxx -I/usr/include/python2.7 -I../src -DHAVE_CONFIG_H
	g++ -shared $(OBJ) cc_wrap.o -o _cc.so

clean:
	rm -f cc_wrap.cxx _cc.so cc.py cc.pyc $(OBJ) cc_wrap.o
