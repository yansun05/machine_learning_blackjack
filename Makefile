# Author: Yan Sun
# Copyright (c) 2011 All Right Reserved, Yan Sun.

# Rule for building a .o file from a .cpp source file -------
.SUFFIXES: .cpp
.cpp.o:
	c++ -c $(CXXFLAGS) $<

# Compile with debug option and all warnings on. ------------
CXXFLAGS = -g -Wall

# Need library librt for clock_gettime()
LDFLAGS = -lrt

# Object modules comprising this application ----------------
OBJ = main.o player.o deck.o game.o card.o learning.o

blackjack: $(OBJ)
	c++  -o $@ $(CXXFLAGS) $(OBJ) $(LDFLAGS)

# Delete .o and exe files and force recompilation. ----------
clean:
	rm -f $(OBJ) blackjack

# Dependencies ----------------------------------------------
player.o: player.cpp player.h card.h game.h learning.h learning.cpp
deck.o: deck.h deck.cpp card.h
main.o: main.cpp game.h
game.o: game.cpp game.h deck.h player.h
card.o:	card.h card.cpp
learning.o: player.cpp player.h card.h game.h learning.h learning.cpp

