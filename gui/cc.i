// SWIG interface
%module cc
%{
#include "../src/play.hh"
#include "../src/board.hh"
#include "../src/game.hh"
#include "../src/cell.hh"
%}

%include "std_string.i"

%include ../src/cell.hh
%include ../src/cell.hh
%include ../src/player.hh
%include ../src/cell.hh
%include ../src/util.hh
%include ../src/play.hh
%include ../src/board.hh
%include ../src/game.hh
